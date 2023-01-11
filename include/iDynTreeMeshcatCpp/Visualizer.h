/**
 * @file Visualizer.h
 * @authors Giulio Romualdi
 * @copyright This software may be modified and distributed under the terms of the BSD-3-Clause
 * license.
 */

#ifndef IDYNTREE_MESHCAT_CPP_VIEWER_H
#define IDYNTREE_MESHCAT_CPP_VIEWER_H

#include <iDynTree/Model/Model.h>
#include <memory>

namespace iDynTreeMeshcatCpp
{
class Visualizer
{
public:
    Visualizer();
    ~Visualizer();

    bool loadModel(const iDynTree::Model& model, //
                   std::string modelName,
                   const std::vector<uint8_t>& color);

    void join();

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
};
} // namespace iDynTreeMeshcatCpp

#endif // IDYNTREE_MESHCAT_CPP_VIEWER_H
