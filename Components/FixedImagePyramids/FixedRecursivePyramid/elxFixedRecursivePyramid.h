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
#ifndef elxFixedRecursivePyramid_h
#define elxFixedRecursivePyramid_h

#include "elxIncludes.h" // include first to avoid MSVS warning
#include "itkRecursiveMultiResolutionPyramidImageFilter.h"

namespace elastix
{

/**
 * \class FixedRecursivePyramid
 * \brief A pyramid based on the itk::RecursiveMultiResolutionPyramidImageFilter.
 *
 * The parameters used in this class are:
 * \parameter FixedImagePyramid: Select this pyramid as follows:\n
 *    <tt>(FixedImagePyramid "FixedRecursiveImagePyramid")</tt>
 *
 * \ingroup ImagePyramids
 */

template <class TElastix>
class ITK_TEMPLATE_EXPORT FixedRecursivePyramid
  : public itk::RecursiveMultiResolutionPyramidImageFilter<typename FixedImagePyramidBase<TElastix>::InputImageType,
                                                           typename FixedImagePyramidBase<TElastix>::OutputImageType>
  , public FixedImagePyramidBase<TElastix>
{
public:
  /** Standard ITK-stuff. */
  typedef FixedRecursivePyramid Self;
  typedef itk::RecursiveMultiResolutionPyramidImageFilter<typename FixedImagePyramidBase<TElastix>::InputImageType,
                                                          typename FixedImagePyramidBase<TElastix>::OutputImageType>
                                          Superclass1;
  typedef FixedImagePyramidBase<TElastix> Superclass2;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FixedRecursivePyramid, itk::RecursiveMultiResolutionPyramidImageFilter);

  /** Name of this class.
   * Use this name in the parameter file to select this specific pyramid. \n
   * example: <tt>(FixedImagePyramid "FixedRecursiveImagePyramid")</tt>\n
   */
  elxClassNameMacro("FixedRecursiveImagePyramid");

  /** Get the ImageDimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass1::ImageDimension);

  /** Typedefs inherited from the superclass. */
  using typename Superclass1::InputImageType;
  using typename Superclass1::OutputImageType;
  using typename Superclass1::InputImagePointer;
  using typename Superclass1::OutputImagePointer;
  using typename Superclass1::InputImageConstPointer;
  using typename Superclass1::ScheduleType;

  /** Typedefs inherited from Elastix. */
  using typename Superclass2::ElastixType;
  using typename Superclass2::ElastixPointer;
  using typename Superclass2::ConfigurationType;
  using typename Superclass2::ConfigurationPointer;
  using typename Superclass2::RegistrationType;
  using typename Superclass2::RegistrationPointer;
  typedef typename Superclass2::ITKBaseType ITKBaseType;

protected:
  /** The constructor. */
  FixedRecursivePyramid() = default;
  /** The destructor. */
  ~FixedRecursivePyramid() override = default;

private:
  elxOverrideGetSelfMacro;

  /** The deleted copy constructor. */
  FixedRecursivePyramid(const Self &) = delete;
  /** The deleted assignment operator. */
  void
  operator=(const Self &) = delete;
};

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#  include "elxFixedRecursivePyramid.hxx"
#endif

#endif // end #ifndef elxFixedRecursivePyramid_h
