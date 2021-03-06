#include <itkFileOutputWindow.h>
#include <itkScaleTransform.h>

int main(int argc, char* argv[])
{
  typedef itk::FileOutputWindow myFileOutputWindow;
  myFileOutputWindow::Pointer window = myFileOutputWindow::New();

  if (argc > 1)
    {
    window->SetFileName(argv[1]);
    }
  window->FlushOn();

  // Set the singelton instance
  itk::OutputWindow::SetInstance(window);

  // Generic output
  itkGenericOutputMacro("This should be in the file: " << window->GetFileName());
  // Warning
  typedef itk::ScaleTransform<float,2> TransformType;
  TransformType::Pointer transform = TransformType::New();
#if defined(ITK_FIXED_PARAMETERS_ARE_DOUBLE) || defined(ITK_LEGACY_REMOVE) // After 4.8.1
  TransformType::FixedParametersType parameters(3);
#else                                         //Pre 4.8.1
  TransformType::ParametersType parameters(3);
#endif
  transform->SetFixedParameters(parameters);

  std::cout << "Look in " << window->GetFileName() << " for the output" << std::endl;
  return EXIT_SUCCESS;
}
