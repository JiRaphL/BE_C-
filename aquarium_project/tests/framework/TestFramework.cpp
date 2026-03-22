#include "framework/TestFramework.h"

#include <sstream>


namespace testfw
{
   Registry & Registry::instance( void )
   {
      static Registry reg;
      return reg;
   }


   bool Registry::add( const std::string & name, std::function<void( void )> body )
   {
      allTests.push_back( TestCase{ name, body } );
      return true;
   }


   void fail( const char * expr, const char * file, int line, const std::string & detail )
   {
      std::ostringstream oss;
      oss << file << ":" << line << " - " << expr;
      if ( !detail.empty() ) oss << " [" << detail << "]";
      throw AssertionFailure( oss.str() );
   }
}
