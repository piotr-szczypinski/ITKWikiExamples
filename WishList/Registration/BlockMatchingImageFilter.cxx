#include "itkBlockMatchingImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkPoint.h"
#include "itkPointSet.h"

typedef itk::Image<unsigned char, 2> ImageType;

static void CreateImage(ImageType::Pointer image, const unsigned int margin);

int main(int argc, char *argv[])
{
  // Create input images
  ImageType::Pointer fixedImage = ImageType::New();
  CreateImage(fixedImage, 43);

  ImageType::Pointer movingImage = ImageType::New();
  CreateImage(movingImage, 40);

  //  typedef  itk::ImageFileWriter<ImageType> WriterType;
  //  WriterType::Pointer writer = WriterType::New();
  //  writer->SetFileName("input.png");
  //  writer->SetInput(input);
  //  writer->Update();

  //  typedef itk::BlockMatchingImageFilter<ImageType, ImageType, PointSetType> BlockMatchingImageFilterType;
  typedef itk::BlockMatchingImageFilter<ImageType> BlockMatchingImageFilterType;
  BlockMatchingImageFilterType::Pointer blockMatchingImageFilter =
          BlockMatchingImageFilterType::New();

  // Generate feature points
  typedef BlockMatchingImageFilterType::FeaturePointsType   PointSetType;
  typedef PointSetType::PointType PointType;
  typedef PointSetType::PointsContainerPointer PointsContainerPointer;

  PointSetType::Pointer   pointSet = PointSetType::New();
  PointsContainerPointer  points = pointSet->GetPoints();

  PointType p0, p1, p2, p3;

  p0[0]=  40.0; p0[1]= 40.0;
  p1[0]=  40.0; p1[1]= 60.0;
  p2[0]=  60.0; p2[1]= 40.0;
  p3[0]=  60.0; p3[1]= 60.0;

  points->InsertElement(0, p0);
  points->InsertElement(1, p1);
  points->InsertElement(2, p2);
  points->InsertElement(3, p3);

  // Sets block radius
  ImageType::SizeType radius;
  radius.Fill(2);
  blockMatchingImageFilter->SetBlockRadius(radius);
  // Sets search radius
  radius.Fill(5);
  blockMatchingImageFilter->SetSearchRadius(radius);

  // Sets the images and landmarks
  blockMatchingImageFilter->SetFixedImage(fixedImage);
  blockMatchingImageFilter->SetMovingImage(movingImage);
  blockMatchingImageFilter->SetFeaturePoints(pointSet);
  //blockMatchingImageFilter->UpdateLargestPossibleRegion();
  blockMatchingImageFilter->Update();

  // Gets results
  typename BlockMatchingImageFilterType::DisplacementsType * displacements =
          blockMatchingImageFilter->GetDisplacements();
  typename BlockMatchingImageFilterType::SimilaritiesType * similarities =
          blockMatchingImageFilter->GetSimilarities();

  std::cout << "There are " << displacements->GetNumberOfPoints() << " landmarks:" << std::endl;

  // Prints resulting displacement vectors and similarity measures
  for(unsigned int n = 0; n < displacements->GetNumberOfPoints(); n++)
    {
    std::cout << "\t" << n << ". "
              << displacements->GetPoint(n)
              << " Displacement=" << displacements->GetPointData()->ElementAt(n)
              << " Similarity=" << similarities->GetPointData()->ElementAt(n)
              << std::endl;
    }

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image, const unsigned int margin)
{
  // Allocate empty image
  itk::Index<2> start; start.Fill(0);
  itk::Size<2> size; size.Fill(100);
  ImageType::RegionType region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(30);

  // Make a square in the middle of the image
  for(unsigned int r = margin; r < 100-margin; r++)
    {
    for(unsigned int c = margin; c < 100-margin; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;
      image->SetPixel(pixelIndex, 225);
      }
    }
}

