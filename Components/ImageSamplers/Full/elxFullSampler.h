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
#ifndef elxFullSampler_h
#define elxFullSampler_h

#include "elxIncludes.h" // include first to avoid MSVS warning
#include "itkImageFullSampler.h"

namespace elastix
{

/**
 * \class FullSampler
 * \brief An interpolator based on the itk::ImageFullSampler.
 *
 * This image sampler samples all voxels in
 * the InputImageRegion.
 *
 * This sampler does not react to the NewSamplesEveryIteration parameter.
 *
 * The parameters used in this class are:
 * \parameter ImageSampler: Select this image sampler as follows:\n
 *    <tt>(ImageSampler "Full")</tt>
 *
 * \ingroup ImageSamplers
 */

template <class TElastix>
class ITK_TEMPLATE_EXPORT FullSampler
  : public itk::ImageFullSampler<typename elx::ImageSamplerBase<TElastix>::InputImageType>
  , public elx::ImageSamplerBase<TElastix>
{
public:
  /** Standard ITK-stuff. */
  typedef FullSampler                                                                     Self;
  typedef itk::ImageFullSampler<typename elx::ImageSamplerBase<TElastix>::InputImageType> Superclass1;
  typedef elx::ImageSamplerBase<TElastix>                                                 Superclass2;
  typedef itk::SmartPointer<Self>                                                         Pointer;
  typedef itk::SmartPointer<const Self>                                                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FullSampler, itk::ImageFullSampler);

  /** Name of this class.
   * Use this name in the parameter file to select this specific interpolator. \n
   * example: <tt>(ImageSampler "Full")</tt>\n
   */
  elxClassNameMacro("Full");

  /** Typedefs inherited from the superclass. */
  using typename Superclass1::DataObjectPointer;
  using typename Superclass1::OutputVectorContainerType;
  using typename Superclass1::OutputVectorContainerPointer;
  using typename Superclass1::InputImageType;
  using typename Superclass1::InputImagePointer;
  using typename Superclass1::InputImageConstPointer;
  using typename Superclass1::InputImageRegionType;
  using typename Superclass1::InputImagePixelType;
  using typename Superclass1::ImageSampleType;
  using typename Superclass1::ImageSampleContainerType;
  using typename Superclass1::MaskType;
  using typename Superclass1::InputImageIndexType;
  using typename Superclass1::InputImagePointType;

  /** The input image dimension. */
  itkStaticConstMacro(InputImageDimension, unsigned int, Superclass1::InputImageDimension);

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
  FullSampler() = default;
  /** The destructor. */
  ~FullSampler() override = default;

private:
  elxOverrideGetSelfMacro;

  /** The deleted copy constructor. */
  FullSampler(const Self &) = delete;
  /** The deleted assignment operator. */
  void
  operator=(const Self &) = delete;
};

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#  include "elxFullSampler.hxx"
#endif

#endif // end #ifndef elxFullSampler_h
