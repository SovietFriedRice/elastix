#ifndef __xoutcell_hxx
#define __xoutcell_hxx

#include "xoutcell.h"

namespace xoutlibrary
{
	using namespace std;

	/**
	 * ************************ Constructor *************************
	 */

	template< class charT, class traits >
		xoutcell<charT, traits>::xoutcell()
	{		
		this->AddTargetCell( "InternalBuffer", &(this->m_InternalBuffer) );

	} // end Constructor


	/**
	 * ********************* Destructor *****************************
	 */

	template< class charT, class traits >
		xoutcell<charT, traits>::~xoutcell()
	{
		//nothing

	} // end Destructor


	/**
	 * ******************** WriteBufferedData ***********************
	 * 
	 * The buffered data is sent to the outputs.
	 */

	template< class charT, class traits >
		void xoutcell<charT, traits>::WriteBufferedData(void)
	{
		/** Make sure all data is written to the string */
		this->m_InternalBuffer << flush;
		
		const std::string & strbuf = this->m_InternalBuffer.str();

		const char * charbuf = strbuf.c_str();

		/** Send the string to the outputs */
		for ( CStreamMapIteratorType cit = this->m_COutputs.begin();
			cit != this->m_COutputs.end(); ++cit )
		{
			*(cit->second) << charbuf << flush;
		}
			
		/** Send the string to the outputs */
		for ( XStreamMapIteratorType xit = this->m_XOutputs.begin();
			xit != this->m_XOutputs.end(); ++xit )
		{
			*(xit->second) << charbuf;
			xit->second->WriteBufferedData();
		}

		/** Empty the internal buffer */
		this->m_InternalBuffer.str( string("") );

	} // end WriteBufferedData



} // end namespace xoutlibrary


#endif // end #ifndef __xoutcell_hxx

