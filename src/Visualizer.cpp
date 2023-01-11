/**
 * @file Visualizer.cpp
 * @authors Giulio Romualdi
 * @copyright This software may be modified and distributed under the terms of the BSD-3-Clause
 * license.
 */

#include <MeshcatCpp/Material.h>
#include <MeshcatCpp/Meshcat.h>
#include <MeshcatCpp/Shape.h>

#include <iDynTree/Core/MatrixFixSize.h>
#include <iDynTree/Core/Transform.h>
#include <iDynTree/Core/VectorFixSize.h>
#include <iDynTree/Model/Model.h>
#include <iDynTree/Model/SolidShapes.h>
#include <iDynTreeMeshcatCpp/Visualizer.h>

#include <iDynTree/Core/VectorDynSize.h>
#include <iDynTree/Model/ForwardKinematics.h>
#include <iDynTree/Model/LinkState.h>
#include <iDynTree/Model/Traversal.h>

#include <memory>
#include <string_view>
#include <utility>

namespace iDynTreeMeshcatCpp
{

struct Visualizer::Impl
{
    ::MeshcatCpp::Meshcat meshcat;

    struct ModelData
    {
        iDynTree::Model model;
        iDynTree::Traversal traversal;
        iDynTree::LinkPositions linkPositions;
    };

    std::unordered_map<std::string, ModelData> storedModels;

    [[nodiscard]] inline bool modelExists(const std::string& modelName)
    {
        return this->storedModels.find(modelName) != this->storedModels.end();
    }

    [[nodiscard]] inline bool isMesh(const iDynTree::SolidShape& geometry)
    {
        if (!geometry.isExternalMesh())
        {
            return false;
        }

        std::string_view meshPath = geometry.asExternalMesh()->getFilename();

        if (meshPath.size() == 0)
            return false;

        std::size_t pos = meshPath.find_last_of(".");
        if (pos == std::string::npos)
        {
            return false;
        }
        const std::string_view format = meshPath.substr(pos + 1);

        return format == "dae" || format == "stl" || format == "obj";
    }

    void addModelGeometryToView(ModelData& data,
                                std::string_view modelName,
                                const std::vector<uint8_t>& color)
    {

        iDynTree::Model& model = data.model;
        iDynTree::Traversal& traversal = data.traversal;
        iDynTree::LinkPositions& linkPositions = data.linkPositions;

        iDynTree::VectorDynSize tmpVector(model.getNrOfJoints());
        tmpVector.zero();

        if (!iDynTree::ForwardPositionKinematics(model,
                                                 traversal,
                                                 iDynTree::Transform::Identity(),
                                                 tmpVector,
                                                 linkPositions))
        {
            return;
        }

        const iDynTree::ModelSolidShapes& visualSolidShapes = model.visualSolidShapes();
        const auto& linksSolidShapes = visualSolidShapes.getLinkSolidShapes();

        int linkIndex = 0;
        for (const auto& linkSolidShapes : linksSolidShapes)
        {
            const iDynTree::Transform world_H_frame = linkPositions(linkIndex);
            const std::string linkName = model.getLinkName(linkIndex);

            for (auto linkSolidShape : linkSolidShapes)
            {
                assert(linkSolidShape);

                if (!this->isMesh(*linkSolidShape))
                {
                    continue;
                }

                iDynTree::ExternalMesh* externalMesh = linkSolidShape->asExternalMesh();

                const std::string fileName = externalMesh->getFileLocationOnLocalFileSystem();
                std::size_t pos_dot = fileName.find_last_of(".");
                std::size_t pos_slash = fileName.find_last_of("/");
                if (pos_slash == std::string::npos)
                {
                    pos_slash = 0;
                }

                const std::string viewerName
                    = std::string(modelName) + "/" + linkName + "/"
                      + fileName.substr(pos_slash + 1, pos_dot - pos_slash - 1);

                MeshcatCpp::Mesh mesh(externalMesh->getFileLocationOnLocalFileSystem(),
                                      externalMesh->getScale()(0));
                MeshcatCpp::Material material = MeshcatCpp::Material::get_default_material();

                const iDynTree::Vector4 color = externalMesh->getMaterial().color();
                material.set_color(color(0) * 255, color(1) * 255, color(2) * 255);

                this->meshcat.set_object(viewerName, mesh, material);

                const iDynTree::Transform transform
                    = (world_H_frame * linkSolidShape->getLink_H_geometry());
                this->meshcat.set_transform(viewerName, transform.asHomogeneousTransform());
            }
            linkIndex++;
        }
    }
};

Visualizer::Visualizer()
{
    m_pimpl = std::make_unique<Visualizer::Impl>();
}

Visualizer::~Visualizer() = default;

bool Visualizer::loadModel(const iDynTree::Model& model, //
                           std::string modelName,
                           const std::vector<uint8_t>& color)
{
    if (m_pimpl->modelExists(modelName))
    {
        return false;
    }

    Impl::ModelData data;
    m_pimpl->storedModels[modelName].model = model;
    auto& storedModel = m_pimpl->storedModels[modelName];
    storedModel.model.computeFullTreeTraversal(storedModel.traversal);
    storedModel.linkPositions.resize(storedModel.model);

    m_pimpl->addModelGeometryToView(storedModel, modelName, color);

    return true;
}

void Visualizer::join()
{
    m_pimpl->meshcat.join();
}

} // namespace iDynTreeMeshcatCpp
