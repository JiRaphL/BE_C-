#ifndef _TEST_FRAMEWORK_H_
#define _TEST_FRAMEWORK_H_

#include <cmath>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


namespace testfw
{
   struct TestCase
   {
      std::string name;
      std::function<void( void )> body;
   };

   class Registry
   {
   public:
      static Registry & instance( void );

      bool add( const std::string & name, std::function<void( void )> body );
      const std::vector<TestCase> & tests( void ) const { return allTests; }

   private:
      std::vector<TestCase> allTests;
   };

   class AssertionFailure : public std::runtime_error
   {
   public:
      explicit AssertionFailure( const std::string & msg ) : std::runtime_error( msg ) {}
   };

   void fail( const char * expr, const char * file, int line, const std::string & detail = "" );

   template<typename T, typename U>
   void expectEqual( const T & actual, const U & expected, const char * aExpr, const char * eExpr, const char * file, int line )
   {
      if ( !( actual == expected ) )
      {
         std::ostringstream oss;
         oss << aExpr << " != " << eExpr;
         fail( "EXPECT_EQ", file, line, oss.str() );
      }
   }

   inline void expectNear( double actual, double expected, double eps, const char * file, int line )
   {
      if ( std::fabs( actual - expected ) > eps )
      {
         std::ostringstream oss;
         oss << "abs(" << actual << " - " << expected << ") > " << eps;
         fail( "EXPECT_NEAR", file, line, oss.str() );
      }
   }
}


#define TEST_CASE(name)                                                                                                 \
   static void name( void );                                                                                            \
   static bool name##_registered = ::testfw::Registry::instance().add( #name, &name );                                \
   static void name( void )

#define EXPECT_TRUE(cond)                                                                                               \
   do {                                                                                                                 \
      if ( !( cond ) ) ::testfw::fail( #cond, __FILE__, __LINE__ );                                                    \
   } while ( false )

#define EXPECT_EQ(actual, expected)                                                                                     \
   do {                                                                                                                 \
      ::testfw::expectEqual( ( actual ), ( expected ), #actual, #expected, __FILE__, __LINE__ );                      \
   } while ( false )

#define EXPECT_NEAR(actual, expected, eps)                                                                              \
   do {                                                                                                                 \
      ::testfw::expectNear( ( actual ), ( expected ), ( eps ), __FILE__, __LINE__ );                                  \
   } while ( false )


#endif
