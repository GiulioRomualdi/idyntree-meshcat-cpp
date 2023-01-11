find_package(icub-models REQUIRED)

add_executable(example example_viewer.cpp)
target_link_libraries(example
  ${PROJECT_NAME}::${PROJECT_NAME}
  icub-models::icub-models
  iDynTree::idyntree-modelio)
