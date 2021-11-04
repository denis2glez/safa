#ifndef OBSERVERREADER_H_
#define OBSERVERREADER_H_

#include <Common/itkReaderEvents.h>
using namespace itk;

template<class T>
class ObserverReader : public itk::Command
{
  public:
    typedef  ObserverReader   Self;
    typedef  itk::Command             Superclass;
    typedef  itk::SmartPointer<ObserverReader>  Pointer;
    itkNewMacro( ObserverReader );
  protected:
    ObserverReader() {};
  public:

  void Execute(itk::Object *caller, const itk::EventObject & event)
  {
    Execute( (const itk::Object *)caller, event);
  }

  void Execute(const itk::Object * object, const itk::EventObject & event)
  {
    const T * reader =
      dynamic_cast< const T * >( object );

    std::cout << event.GetEventName() << ": " << reader->GetProgress() << "\n";
  }
};

#endif /* OBSERVERREADER_H_ */
