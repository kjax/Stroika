/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Database_ODBCClient_h_
#define	_Stroika_Foundation_Database_ODBCClient_h_	1

#include	"../StroikaPreComp.h"

#ifdef _WIN32
#include	<Windows.h>
#endif

#include	<string>

#include	"../Characters/StringUtils.h"
#include	"../Execution/Exceptions.h"
#include	"../Memory/RefCntPtr.h"

namespace	Stroika {	
	namespace	Foundation {
		namespace	Database {


	using	namespace	std;
	using	namespace	Stroika::Foundation;
	using	namespace	Stroika::Foundation::Execution;
	using	namespace	Stroika::Foundation::Memory;

	class	Exception : public StringException {
		public:
			Exception (const wstring& message);
	};
	class	NoDataException : public Exception {
		public:
			NoDataException ();
	};

	class	DBConnection {
		private:
			class	Rep;
		public:
			DBConnection (const wstring& dsn);
			virtual ~DBConnection () ;
		
		public:
			nonvirtual	unsigned int GetNestedTransactionCount () const;

		private:
			RefCntPtr<Rep>	fRep;
	};


	// Maybe pattern this more after an 'iterator'?
	class	Query {
		public:
			class	AbstractColumn;
		private:
			class	Rep;
		public:
			Query (DBConnection database);

		public:
			void	Bind (RefCntPtr<AbstractColumn>* columns, size_t numberColumns);

		public:
			void	Execute (const wstring& sqlQuery);

		public:
			bool	FetchRow ();

		private:
			RefCntPtr<Rep>	fRep;
	};

		}
	}
}





/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */

#endif	/*_Stroika_Foundation_Database_ODBCClient_h_*/