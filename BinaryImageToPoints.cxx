/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// This program reads a binary image and converts the black pixels to points
// in a vtkPolyData. Drawing in an image is an easier way to create a data set
// for testing FindBoundary and BoundingPolygon than specifying 3D points
// manually.

// VTK
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkVertexGlyphFilter.h>

// ITK
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageRegionConstIterator.h>

typedef itk::Image<unsigned char, 2> ImageType;

std::vector<itk::Index<2> > BinaryImageToPixelList(ImageType::Pointer image);
void WritePoints(vtkPolyData* polydata, std::string filename);
void PixelListToPolyData(std::vector<itk::Index<2> > pixelList, vtkSmartPointer<vtkPolyData> polydata);

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required arguments: inputFileName.png outputFileName.vtp" << std::endl;
    return EXIT_FAILURE;
    }
    
  std::string inputFileName = argv[1];
  std::cout << "Reading " << inputFileName << std::endl;

  std::string outputFileName = argv[2];
  
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();
  
  std::vector<itk::Index<2> > pixelList = BinaryImageToPixelList(reader->GetOutput());
  std::cout << "pixelList has " << pixelList.size() << " points." << std::endl;
  
  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
  PixelListToPolyData(pixelList, polydata);
  std::cout << "polydata has " << polydata->GetNumberOfPoints() << " points." << std::endl;
  
  WritePoints(polydata, outputFileName.c_str());
  
  return EXIT_SUCCESS;
}


std::vector<itk::Index<2> > BinaryImageToPixelList(ImageType::Pointer image)
{
  std::vector<itk::Index<2> > pixelList;

  itk::ImageRegionConstIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.Get() == 0) // All black pixels are considered points
      {
      pixelList.push_back(imageIterator.GetIndex());
      }

    ++imageIterator;
    }
    
  //std::cout << "There are " << pixelList.size() << " points." << std::endl;
  
  return pixelList;
}

void WritePoints(vtkPolyData* polydata, std::string filename)
{
  // Write the points ina polydata to a vtp file
  
  vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
    vtkSmartPointer<vtkVertexGlyphFilter>::New();
  glyphFilter->SetInputConnection(polydata->GetProducerPort());
  glyphFilter->Update();
  
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName(filename.c_str());
  writer->SetInput(glyphFilter->GetOutput());
  writer->Write();
}


void PixelListToPolyData(std::vector<itk::Index<2> > pixelList, vtkSmartPointer<vtkPolyData> polydata)
{
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  for(unsigned int i = 0; i < pixelList.size(); i++)
    {
    points->InsertNextPoint(pixelList[i][0], pixelList[i][1], 0);
    //std::cout << pixelList[i][0] << " " << pixelList[i][1] << std::endl;
    }
  polydata->SetPoints(points);
  //std::cout << "There are " << polydata->GetNumberOfPoints() << " points." << std::endl;
}

