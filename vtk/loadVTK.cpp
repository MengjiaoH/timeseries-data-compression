#include "loadVTK.h"

void loadVTK(std::string file, std::vector<float> &volume, ospcommon::math::vec3i &dims)
{
    std::cout << "Start Loading " << file << std::endl;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    vtkSmartPointer<vtkStructuredGridReader> reader = vtkSmartPointer<vtkStructuredGridReader>::New();
    reader ->SetFileName(file.c_str());
    reader ->Update();
    auto grid = reader ->GetOutput();
    //Get Dimensions
    int dim[3] = {0, 0, 0};
    grid -> GetDimensions(dim);
    std::cout << " Dimensions " << dim[0] << " " << dim[1] << " " << dim[2] << std::endl;
    dims[0] = dim[0]; dims[1] = dim[1]; dims[2] = dim[2]; 
    // volume.dim.x = dim[0]; volume.dim.y = dim[1]; volume.dim.z = dim[2];
    //Get Data Array
    vtkSmartPointer<vtkGeometryFilter> geometryFilter = vtkSmartPointer<vtkGeometryFilter>::New();
    geometryFilter ->SetInputData(grid);
    geometryFilter ->Update();

    vtkPolyData* polydata = geometryFilter ->GetOutput();
    // vtkIdType numberofPointArray = polydata ->GetPointData() ->GetNumberOfArrays();
    // for(vtkIdType i = 0; i < numberofPointArray; i++){
    //     int dataTypeID = polydata ->GetPointData() ->GetArray(i) ->GetDataType();
    //     std::cout << "Array " << i << ": " << polydata ->GetPointData() ->GetArrayName(i)
    //               << " (type: " << dataTypeID << " )" << std::endl;
    // }
    std::string arrayName = "Magnitude";
    // std::string arrayName = "Directions";
    vtkSmartPointer<vtkFloatArray> array = vtkFloatArray::SafeDownCast(polydata->GetPointData()->GetArray(arrayName.c_str()));
    vtkIdType idNumPointsInFile = polydata->GetNumberOfPoints();
    if(array){
        std::cout << "Got array " << arrayName
                << " with " << idNumPointsInFile << " values"
                << std::endl;
        for(int i = 0; i < idNumPointsInFile; i++){
            float value;
            value = array->GetValue(i);
            volume.push_back(value);
        }
    }else{
        std::cout << "The file " << file
                << " does not have a PointData array named " << arrayName
                << std::endl;
    }
    // remove 2 slices to make z dividable by 4
    int size = idNumPointsInFile - dim[0] * dim[1] * 2;
    volume.erase(volume.begin() + size, volume.end());
    // std::cout << "size of volume = " << volume.size() << std::endl;

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took " << time_span.count() << " seconds loading one time step." << std::endl;
}
