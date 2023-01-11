/**
 * @file example_viewer.h
 * @authors Giulio Romualdi
 * @copyright This software may be modified and distributed under the terms of the BSD-3-Clause
 * license.
 */

#include <iDynTreeMeshcatCpp/Visualizer.h>
#include <iDynTree/Model/Model.h>
#include <iDynTree/ModelIO/ModelLoader.h>
#include <iCubModels/iCubModels.h>

int main()
{
    iDynTreeMeshcatCpp::Visualizer viz;

    const std::string modelPath = iCubModels::getModelFile("iCubGenova09");
    iDynTree::ModelLoader loader;
    loader.loadModelFromFile(modelPath);

    viz.loadModel(loader.model(), "iCub", {0,0,0});

    viz.join();

    return 0;
}
