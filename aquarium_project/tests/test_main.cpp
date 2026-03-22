#include "framework/TestFramework.h"

#include <exception>
#include <iostream>


int main( void )
{
   const std::vector<testfw::TestCase> & tests = testfw::Registry::instance().tests();
   int failures = 0;

   for ( std::vector<testfw::TestCase>::const_iterator it = tests.begin(); it != tests.end(); ++it )
   {
      try
      {
         it->body();
         std::cout << "[PASS] " << it->name << std::endl;
      }
      catch ( const std::exception & ex )
      {
         ++failures;
         std::cout << "[FAIL] " << it->name << " -> " << ex.what() << std::endl;
      }
      catch ( ... )
      {
         ++failures;
         std::cout << "[FAIL] " << it->name << " -> unknown error" << std::endl;
      }
   }

   std::cout << "Executed " << tests.size() << " test(s), failures: " << failures << std::endl;
   return failures == 0 ? 0 : 1;
}
