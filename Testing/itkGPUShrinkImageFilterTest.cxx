/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkTestHelper.h"

// GPU includes
#include "itkGPUImageFactory.h"
#include "itkGPUShrinkImageFilterFactory.h"

// ITK include files
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkTimeProbe.h"

#include <iomanip> // setprecision, etc.

//------------------------------------------------------------------------------
// This test compares the CPU with the GPU version of the ShrinkImageFilter.
// The filter takes an input image and produces an output image.
// We compare the CPU and GPU output image write RMSE and speed.

int
main(int argc, char * argv[])
{
  // Check arguments for help
  if (argc < 4)
  {
    std::cerr << "ERROR: insufficient command line arguments.\n"
              << "  inputFileName outputNameCPU outputNameGPU" << std::endl;
    return EXIT_FAILURE;
  }

  // Setup for debugging
  itk::SetupForDebugging();

  // Create and check OpenCL context
  if (!itk::CreateContext())
  {
    return EXIT_FAILURE;
  }

  /** Get the command line arguments. */
  const std::string  inputFileName = argv[1];
  const std::string  outputFileNameCPU = argv[2];
  const std::string  outputFileNameGPU = argv[3];
  const unsigned int shrinkFactor = 2;
  const double       epsilon = 1e-3;
  const unsigned int runTimes = 5;

  std::cout << std::showpoint << std::setprecision(4);

  // Typedefs.
  const unsigned int                       Dimension = 3;
  typedef float                            PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;

  // CPU Typedefs
  typedef itk::ShrinkImageFilter<ImageType, ImageType> FilterType;
  typedef itk::ImageFileReader<ImageType>              ReaderType;
  typedef itk::ImageFileWriter<ImageType>              WriterType;

  // Reader
  auto reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  // Construct the filter
  auto cpuFilter = FilterType::New();
  cpuFilter->SetShrinkFactors(shrinkFactor);

  std::cout << "Testing the ShrinkImageFilter, CPU vs GPU:\n";
  std::cout << "CPU/GPU shrinkFactor #threads time speedup RMSE\n";

  // Time the filter, run on the CPU
  itk::TimeProbe cputimer;
  cputimer.Start();
  for (unsigned int i = 0; i < runTimes; ++i)
  {
    cpuFilter->SetInput(reader->GetOutput());
    try
    {
      cpuFilter->Update();
    }
    catch (itk::ExceptionObject & e)
    {
      std::cerr << "ERROR: " << e << std::endl;
      itk::ReleaseContext();
      return EXIT_FAILURE;
    }

    // Modify the filter, only not the last iteration
    if (i != runTimes - 1)
    {
      cpuFilter->Modified();
    }
  }
  cputimer.Stop();

  std::cout << "CPU " << shrinkFactor << " " << cpuFilter->GetNumberOfWorkUnits() << " "
            << cputimer.GetMean() / runTimes << std::endl;

  /** Write the CPU result. */
  auto writer = WriterType::New();
  writer->SetInput(cpuFilter->GetOutput());
  writer->SetFileName(outputFileNameCPU.c_str());
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject & e)
  {
    std::cerr << "ERROR: " << e << std::endl;
    itk::ReleaseContext();
    return EXIT_FAILURE;
  }

  // Register object factory for GPU image and filter
  // All these filters that are constructed after this point are
  // turned into a GPU filter.
  typedef typelist::MakeTypeList<float>::Type OCLImageTypes;
  itk::GPUImageFactory2<OCLImageTypes, OCLImageDims>::RegisterOneFactory();
  itk::GPUShrinkImageFilterFactory2<OCLImageTypes, OCLImageTypes, OCLImageDims>::RegisterOneFactory();

  // Construct the filter
  // Use a try/catch, because construction of this filter will trigger
  // OpenCL compilation, which may fail.
  FilterType::Pointer gpuFilter;
  try
  {
    gpuFilter = FilterType::New();
    itk::ITKObjectEnableWarnings(gpuFilter.GetPointer());
  }
  catch (itk::ExceptionObject & e)
  {
    std::cerr << "ERROR: " << e << std::endl;
    itk::ReleaseContext();
    return EXIT_FAILURE;
  }
  gpuFilter->SetShrinkFactors(shrinkFactor);

  // Also need to re-construct the image reader, so that it now
  // reads a GPUImage instead of a normal image.
  // Otherwise, you will get an exception when running the GPU filter:
  // "ERROR: The GPU InputImage is NULL. Filter unable to perform."
  auto gpuReader = ReaderType::New();
  gpuReader->SetFileName(inputFileName);

  // \todo: If the following line is uncommented something goes wrong with
  // the ITK pipeline synchronization.
  // Something is still read in, but I guess it is not properly copied to
  // the GPU. The output of the shrink filter is then bogus.
  // The following line is however not needed in a pure CPU implementation.
  gpuReader->Update();

  // Time the filter, run on the GPU
  itk::TimeProbe gputimer;
  gputimer.Start();
  for (unsigned int i = 0; i < runTimes; ++i)
  {
    gpuFilter->SetInput(gpuReader->GetOutput());
    try
    {
      gpuFilter->Update();
    }
    catch (itk::ExceptionObject & e)
    {
      std::cerr << "ERROR: " << e << std::endl;
      itk::ReleaseContext();
      return EXIT_FAILURE;
    }
    // Modify the filter, only not the last iteration
    if (i != runTimes - 1)
    {
      gpuFilter->Modified();
    }
  }
  gputimer.Stop();

  std::cout << "GPU " << shrinkFactor << " x " << gputimer.GetMean() / runTimes << " "
            << cputimer.GetMean() / gputimer.GetMean();

  /** Write the GPU result. */
  auto gpuWriter = WriterType::New();
  gpuWriter->SetInput(gpuFilter->GetOutput());
  gpuWriter->SetFileName(outputFileNameGPU.c_str());
  try
  {
    gpuWriter->Update();
  }
  catch (itk::ExceptionObject & e)
  {
    std::cerr << "ERROR: " << e << std::endl;
    itk::ReleaseContext();
    return EXIT_FAILURE;
  }

  // Compute RMSE
  double       RMSrelative = 0.0;
  const double RMSerror =
    itk::ComputeRMSE<double, ImageType, ImageType>(cpuFilter->GetOutput(), gpuFilter->GetOutput(), RMSrelative);
  std::cout << " " << RMSerror << std::endl;

  // Check
  if (RMSerror > epsilon)
  {
    std::cerr << "ERROR: RMSE between CPU and GPU result larger than expected" << std::endl;
    itk::ReleaseContext();
    return EXIT_FAILURE;
  }

  // End program.
  itk::ReleaseContext();
  return EXIT_SUCCESS;
} // end main()
