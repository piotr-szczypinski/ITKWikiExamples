#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkImageFileReader.h"

#include "QuickView.h"

int main( int argc, char *argv[])
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage [iterations]" << std::endl;
    return EXIT_FAILURE;
    }

  int iterations = 5;
  if (argc > 2)
    {
    iterations = atoi(argv[2]);
    }

  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 2;
  typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

  typedef  itk::ImageFileReader< InternalImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType >CurvatureFlowImageFilterType;

  CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

  smoothing->SetInput( reader->GetOutput() );
  smoothing->SetNumberOfIterations( iterations );
  smoothing->SetTimeStep( 0.125 );

  typedef itk::SubtractImageFilter< InternalImageType > SubtractImageFilterType;
  SubtractImageFilterType::Pointer diff = SubtractImageFilterType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(smoothing->GetOutput());

  QuickView viewer;
  viewer.AddImage<InternalImageType>(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "CurvatureFlow\niterations = " << iterations;
  viewer.AddImage<InternalImageType>(
    smoothing->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - CurvatureFlow";
  viewer.AddImage<InternalImageType>(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
