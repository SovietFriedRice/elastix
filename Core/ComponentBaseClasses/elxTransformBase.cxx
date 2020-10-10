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

#include "elxElastixTemplate.h"
#include "elxTransformBase.hxx"

#include <itkImage.h>
#include "elxTransformBase.hxx"


using ElastixType1 = elastix::ElastixTemplate < itk::Image<float>, itk::Image<float>>;
using ElastixType2 = elastix::ElastixTemplate < itk::Image<short, 3>, itk::Image<short, 3>>;
using ElastixType3 = elastix::ElastixTemplate < itk::Image<float, 3>, itk::Image<float, 3>>;
using ElastixType4 = elastix::ElastixTemplate < itk::Image<short, 4>, itk::Image<short, 4>>;

/*
using ElastixType1 = elastix::ElastixTemplate < elastix::ElastixTypedef<1>::FixedImageType, elastix::ElastixTypedef<1>::MovingImageType>;
using ElastixType2 = elastix::ElastixTemplate < elastix::ElastixTypedef<2>::FixedImageType, elastix::ElastixTypedef<2>::MovingImageType>;
using ElastixType3 = elastix::ElastixTemplate < elastix::ElastixTypedef<3>::FixedImageType, elastix::ElastixTypedef<3>::MovingImageType>;
using ElastixType4 = elastix::ElastixTemplate < elastix::ElastixTypedef<4>::FixedImageType, elastix::ElastixTypedef<4>::MovingImageType>;
*/

namespace elastix
{
  template class TransformBase<ElastixType1>;
  template class TransformBase<ElastixType2>;
  template class TransformBase<ElastixType3>;
  template class TransformBase<ElastixType4>;
}