#ifndef __elxMovingImagePyramidBase_h
#define __elxMovingImagePyramidBase_h

/** Needed for the macros */
#include "elxMacro.h"

#include "elxBaseComponentSE.h"
#include "itkObject.h"

#include "itkMultiResolutionPyramidImageFilter.h"


namespace elastix
{
using namespace itk;

	/**
	 * \class MovingImagePyramidBase
	 * \brief This class is based on the itk::MultiResolutionPyramidImageFilter.
	 *
	 * This class contains all the common functionality for MovingImagePyramids.
	 *
	 * \parameter MovingImagePyramidSchedule: downsampling factors for the image pyramid.\n
	 *    For each dimension, for each resolution level, the downsampling factor of the 
	 *    moving image can be specified.\n
	 *    Syntax for 2D images:\n
   *    <tt>(MovingImagePyramidSchedule <reslevel0,dim0> <reslevel0,dim1> <reslevel1,dim0> <reslevel1,dim1> ...)</tt>
	 *    example: <tt>(MovingImagePyramidSchedule  4 4 2 2 1 1)</tt>\n
	 *    Default: isotropic, halved in each resolution, so, like in the example. If
   *    ImagePyramidSchedule is specified, that schedule is used for both fixed and moving image pyramid.
   * \parameter ImagePyramidSchedule: downsampling factors for fixed and moving image pyramids.\n
	 *    example: <tt>(ImagePyramidSchedule  4 4 2 2 1 1)</tt> \n
   *    Used as a default when MovingImagePyramidSchedule is not specified. If both are omitted,
   *    a default schedule is assumed: isotropic, halved in each resolution, so, like in the example.
	 *
	 * \ingroup ImagePyramids
	 * \ingroup ComponentBaseClasses
	 */

	template <class TElastix>
		class MovingImagePyramidBase : public BaseComponentSE<TElastix>
	{
	public:

		/** Standard ITK-stuff. */
		typedef MovingImagePyramidBase			Self;
		typedef BaseComponentSE<TElastix>		Superclass;

		/** Run-time type information (and related methods). */
		itkTypeMacro( MovingImagePyramidBase, BaseComponentSE );

		/** Typedefs inherited from the superclass. */
		typedef typename Superclass::ElastixType						ElastixType;
		typedef typename Superclass::ElastixPointer					ElastixPointer;
		typedef typename Superclass::ConfigurationType			ConfigurationType;
		typedef typename Superclass::ConfigurationPointer		ConfigurationPointer;
		typedef typename Superclass::RegistrationType				RegistrationType;
		typedef typename Superclass::RegistrationPointer		RegistrationPointer;

		/** Typedefs inherited from Elastix. */
		typedef typename ElastixType::MovingImageType		InputImageType;
		typedef typename ElastixType::MovingImageType		OutputImageType;
		
		/** Other typedef's. */
		typedef MultiResolutionPyramidImageFilter<
			InputImageType, OutputImageType >				ITKBaseType;

		/** Typedef's from ITKBaseType. */
		typedef typename ITKBaseType::ScheduleType					ScheduleType;

		/** Cast to ITKBaseType. */
		virtual ITKBaseType * GetAsITKBaseType(void)
		{
			return dynamic_cast<ITKBaseType *>(this);
		}

		/** Execute stuff before the actual registration:
		 * \li Set the schedule of the moving image pyramid.
		 */
		virtual void BeforeRegistrationBase(void);

		/** Method for setting the schedule. */
		virtual void SetMovingSchedule(void);

	protected:

		/** The constructor. */
		MovingImagePyramidBase() {}
		/** The destructor. */
		virtual ~MovingImagePyramidBase() {}
		
	private:

		/** The private constructor. */
		MovingImagePyramidBase( const Self& );	// purposely not implemented
		/** The private copy constructor. */
		void operator=( const Self& );					// purposely not implemented

	}; // end class MovingImagePyramidBase


} // end namespace elastix



#ifndef ITK_MANUAL_INSTANTIATION
#include "elxMovingImagePyramidBase.hxx"
#endif

#endif // end #ifndef __elxMovingImagePyramidBase_h

