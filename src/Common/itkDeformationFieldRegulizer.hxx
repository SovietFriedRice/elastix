#ifndef __itkDeformationFieldRegulizer_HXX__
#define __itkDeformationFieldRegulizer_HXX__

#include "itkDeformationFieldRegulizer.h"


namespace itk
{
	
	/**
	 * ************************ Constructor	*************************
	 */

	template <class TAnyITKTransform>
		DeformationFieldRegulizer<TAnyITKTransform>
		::DeformationFieldRegulizer()
	{
		/** Initialize. */
		this->m_IntermediaryDeformationFieldTransform = 0;
		this->m_Initialized = false;
				
	} // end Constructor
	

	/**
	 * *********************** Destructor ***************************
	 */

	template <class TAnyITKTransform>
		DeformationFieldRegulizer<TAnyITKTransform>
		::~DeformationFieldRegulizer()
	{
		//nothing
	} // end Destructor


	/**
	 * ********* InitializeIntermediaryDeformationField **************
	 */

	template <class TAnyITKTransform>
		void DeformationFieldRegulizer<TAnyITKTransform>
		::InitializeDeformationFields( void )
	{
		/** Initialize this->m_IntermediaryDeformationFieldTransform. */
		this->m_IntermediaryDeformationFieldTransform = IntermediaryDFTransformType::New();

		/** Initialize this->m_IntermediaryDeformationField. */
		typename VectorImageType::Pointer intermediaryDeformationField = VectorImageType::New();
		intermediaryDeformationField->SetRegions( this->m_DeformationFieldRegion );
		intermediaryDeformationField->SetSpacing( this->m_DeformationFieldSpacing );
		intermediaryDeformationField->SetOrigin( this->m_DeformationFieldOrigin );
		intermediaryDeformationField->Allocate();

		/** Set everything to zero. */
		IteratorType it( intermediaryDeformationField,
			intermediaryDeformationField->GetLargestPossibleRegion() );
		VectorPixelType vec;
		vec.Fill( NumericTraits<ScalarType>::Zero );
		while ( !it.IsAtEnd() )
		{
			it.Set( vec );
			++it;
		}

		/** Set the deformation field in the transform. */
		this->m_IntermediaryDeformationFieldTransform->SetCoefficientImage( intermediaryDeformationField );

		/** Set to initialized. */
		this->m_Initialized = true;

	} // end InitializeDeformationFields


	/**
	 * *********************** TransformPoint ***********************
	 */

	template <class TAnyITKTransform>
		typename DeformationFieldRegulizer<TAnyITKTransform>::OutputPointType
		DeformationFieldRegulizer<TAnyITKTransform>
		::TransformPoint( const InputPointType & inputPoint ) const
	{
		/** Get the outputpoint of any ITK Transform and the deformationfield. */
		OutputPointType oppAnyT, oppDF, opp;
		oppAnyT = this->Superclass::TransformPoint( inputPoint );
		oppDF = this->m_IntermediaryDeformationFieldTransform->TransformPoint( inputPoint );

		/** Add them: don't forget to subtract ipp. */
		for ( unsigned int i = 0; i < OutputSpaceDimension; i++ )
		{
			opp[ i ] = oppAnyT[ i ] + oppDF[ i ] - inputPoint[ i ];
		}

		/** Return a value. */
		return opp;

	} // end TransformPoint


	/**
	 * ******** UpdateIntermediaryDeformationFieldTransform *********
	 */

	template <class TAnyITKTransform>
		void DeformationFieldRegulizer<TAnyITKTransform>
		::UpdateIntermediaryDeformationFieldTransform( typename VectorImageType::Pointer  vecImage )
	{
		/** Set the vecImage (which is allocated elsewhere) and put it in
		 * IntermediaryDeformationFieldTransform (where it is copied and split up).
		 */
		this->m_IntermediaryDeformationFieldTransform->SetCoefficientImage( vecImage );

	} // end UpdateIntermediaryDeformationFieldTransform


} // end namespace itk


#endif // end #ifndef __itkDeformationFieldRegulizer_HXX__

