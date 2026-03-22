#include "Bestiole.h"

#include "Milieu.h"
#include "accessories/IAccessory.h"
#include "accessories/CamouflageAccessory.h"
#include "accessories/FinAccessory.h"
#include "accessories/ShellAccessory.h"
#include "behavior/GregariousBehavior.h"
#include "behavior/IBehavior.h"
#include "sensors/EarSensor.h"
#include "sensors/EyeSensor.h"
#include "sensors/ISensor.h"

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <utility>
#include <cstring>


const double      Bestiole::AFF_SIZE = 8.;
const double      Bestiole::MAX_VITESSE = 10.;
const double      Bestiole::LIMITE_VUE = 30.;

int               Bestiole::next = 0;

namespace
{
   constexpr double PI = 3.14159265358979323846;

   double normaliseAngle( double angle )
   {
      while ( angle > PI ) angle -= 2.0 * PI;
      while ( angle < -PI ) angle += 2.0 * PI;
      return angle;
   }
}


Bestiole::Bestiole( void )
   : Bestiole( std::make_unique<GregariousBehavior>() )
{
}


Bestiole::Bestiole( std::unique_ptr<IBehavior> comportementInit )
{

   identite = ++next;

   cout << "const Bestiole (" << identite << ") par defaut" << endl;

   x = y = 0;
   cumulX = cumulY = 0.;
   orientation = static_cast<double>( rand() )/RAND_MAX*2.*PI;
   vitesse = static_cast<double>( rand() )/RAND_MAX*MAX_VITESSE;
   vitesseBase = vitesse;
   alive = true;
   age = 0;
   maxAge = 200;

   couleur = new T[ 3 ];
   couleur[ 0 ] = static_cast<int>( static_cast<double>( rand() )/RAND_MAX*230. );
   couleur[ 1 ] = static_cast<int>( static_cast<double>( rand() )/RAND_MAX*230. );
   couleur[ 2 ] = static_cast<int>( static_cast<double>( rand() )/RAND_MAX*230. );
   setComportement( std::move( comportementInit ) );
   addSensor( std::make_unique<EyeSensor>( PI / 2.0, LIMITE_VUE, 1.0 ) );

}


Bestiole::Bestiole( const Bestiole & b )
{

   identite = ++next;

   cout << "const Bestiole (" << identite << ") par copie" << endl;

   x = b.x;
   y = b.y;
   cumulX = cumulY = 0.;
   orientation = b.orientation;
   vitesse = b.vitesse;
   vitesseBase = b.vitesseBase;
   alive = b.alive;
   age = b.age;
   maxAge = b.maxAge;
   couleur = new T[ 3 ];
   memcpy( couleur, b.couleur, 3*sizeof(T) );
   if ( b.comportement )
      comportement = b.comportement->clone();
   else
      comportement = std::make_unique<GregariousBehavior>();
   sensors.clear();
   for ( std::vector<std::unique_ptr<ISensor>>::const_iterator it = b.sensors.begin(); it != b.sensors.end(); ++it )
      sensors.push_back( ( *it )->clone() );
   accessories.clear();
   for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = b.accessories.begin(); it != b.accessories.end(); ++it )
      accessories.push_back( ( *it )->clone() );

}


Bestiole & Bestiole::operator=( const Bestiole & b )
{

   if ( this == &b ) return *this;

   x = b.x;
   y = b.y;
   cumulX = cumulY = 0.;
   orientation = b.orientation;
   vitesse = b.vitesse;
   vitesseBase = b.vitesseBase;
   alive = b.alive;
   age = b.age;
   maxAge = b.maxAge;
   memcpy( couleur, b.couleur, 3*sizeof(T) );
   if ( b.comportement )
      comportement = b.comportement->clone();
   else
      comportement = std::make_unique<GregariousBehavior>();
   sensors.clear();
   for ( std::vector<std::unique_ptr<ISensor>>::const_iterator it = b.sensors.begin(); it != b.sensors.end(); ++it )
      sensors.push_back( ( *it )->clone() );
   accessories.clear();
   for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = b.accessories.begin(); it != b.accessories.end(); ++it )
      accessories.push_back( ( *it )->clone() );
   resetVitesse();

   return *this;

}


Bestiole::~Bestiole( void )
{

   delete[] couleur;

   cout << "dest Bestiole" << endl;

}


void Bestiole::initCoords( int xLim, int yLim )
{

   x = rand() % xLim;
   y = rand() % yLim;

}


void Bestiole::setCoords( int nx, int ny )
{

   x = nx;
   y = ny;

}


void Bestiole::bouge( int xLim, int yLim )
{

   double         nx, ny;
   double         dx = cos( orientation )*vitesse;
   double         dy = -sin( orientation )*vitesse;
   int            cx, cy;


   cx = static_cast<int>( cumulX ); cumulX -= cx;
   cy = static_cast<int>( cumulY ); cumulY -= cy;

   nx = x + dx + cx;
   ny = y + dy + cy;

   if ( (nx < 0) || (nx > xLim - 1) ) {
      orientation = PI - orientation;
      cumulX = 0.;
   }
   else {
      x = static_cast<int>( nx );
      cumulX += nx - x;
   }

   if ( (ny < 0) || (ny > yLim - 1) ) {
      orientation = -orientation;
      cumulY = 0.;
   }
   else {
      y = static_cast<int>( ny );
      cumulY += ny - y;
   }

}


void Bestiole::action( Milieu & monMilieu )
{

   if ( !alive )
      return;

   if ( comportement )
      comportement->apply( *this, monMilieu );
   else
      avance( monMilieu );

}


void Bestiole::draw( UImg & support )
{

   if ( !alive )
      return;

   double         xt = x + cos( orientation )*AFF_SIZE/2.1;
   double         yt = y - sin( orientation )*AFF_SIZE/2.1;
   T              drawColor[ 3 ] = { couleur[0], couleur[1], couleur[2] };
   const IBehavior * b = getComportement();

   if ( b )
   {
      const char * name = b->name();
      if ( std::strcmp( name, "Gregarious" ) == 0 )      { drawColor[0] = 70;  drawColor[1] = 180; drawColor[2] = 70;  }
      else if ( std::strcmp( name, "Fearful" ) == 0 )    { drawColor[0] = 220; drawColor[1] = 170; drawColor[2] = 60;  }
      else if ( std::strcmp( name, "Kamikaze" ) == 0 )   { drawColor[0] = 220; drawColor[1] = 60;  drawColor[2] = 60;  }
      else if ( std::strcmp( name, "Cautious" ) == 0 )   { drawColor[0] = 60;  drawColor[1] = 170; drawColor[2] = 220; }
   }

   support.draw_ellipse( x, y, AFF_SIZE, AFF_SIZE/5., -orientation/PI*180., drawColor );
   support.draw_circle( xt, yt, AFF_SIZE/2., drawColor );

    const T eyeMark[ 3 ] = { 20, 40, 220 };
    const T earMark[ 3 ] = { 220, 170, 60 };
    const T finMark[ 3 ] = { 60, 170, 60 };
    const T shellMark[ 3 ] = { 30, 30, 30 };
    const T camoMark[ 3 ] = { 220, 70, 70 };
    int sensorOffset = 0;
    int accessoryOffset = 0;
    for ( std::vector<std::unique_ptr<ISensor>>::const_iterator it = sensors.begin(); it != sensors.end(); ++it )
    {
       const int sx = x - 6 + sensorOffset * 4;
       if ( dynamic_cast<const EyeSensor *>( it->get() ) != nullptr )
          support.draw_circle( sx, y - 10, 1, eyeMark );
       else if ( dynamic_cast<const EarSensor *>( it->get() ) != nullptr )
          support.draw_rectangle( sx - 1, y - 11, sx + 1, y - 9, earMark );
       ++sensorOffset;
    }
    for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = accessories.begin(); it != accessories.end(); ++it )
    {
       const int ax = x - 6 + accessoryOffset * 4;
       if ( dynamic_cast<const FinAccessory *>( it->get() ) != nullptr )
          support.draw_triangle( ax - 1, y + 11, ax + 1, y + 11, ax, y + 9, finMark );
       else if ( dynamic_cast<const ShellAccessory *>( it->get() ) != nullptr )
          support.draw_rectangle( ax - 1, y + 9, ax + 1, y + 11, shellMark );
       else if ( dynamic_cast<const CamouflageAccessory *>( it->get() ) != nullptr )
          support.draw_circle( ax, y + 10, 1, camoMark );
       ++accessoryOffset;
    }

}


bool operator==( const Bestiole & b1, const Bestiole & b2 )
{

   return ( b1.identite == b2.identite );

}


bool Bestiole::jeTeVois( const Bestiole & b ) const
{

   if ( sensors.empty() )
      return false;

   for ( std::vector<std::unique_ptr<ISensor>>::const_iterator it = sensors.begin(); it != sensors.end(); ++it )
   {
      if ( ( *it )->detects( *this, b ) && ( *it )->getDetectionCapacity() > b.getCamouflageLevel() )
         return true;
   }

   return false;

}


void Bestiole::avance( const Milieu & monMilieu )
{

   bouge( monMilieu.getWidth(), monMilieu.getHeight() );

}


void Bestiole::setOrientation( double nouvelleOrientation )
{

   orientation = normaliseAngle( nouvelleOrientation );

}


void Bestiole::ajusterOrientation( double deltaOrientation )
{

   setOrientation( orientation + deltaOrientation );

}


void Bestiole::orienterVers( int cibleX, int cibleY, double intensite )
{

   const double dx = static_cast<double>( cibleX - x );
   const double dy = static_cast<double>( cibleY - y );
   const double orientationCible = std::atan2( -dy, dx );
   const double diff = normaliseAngle( orientationCible - orientation );

   setOrientation( orientation + diff * intensite );

}


void Bestiole::orienterOppose( int cibleX, int cibleY, double intensite )
{

   const double dx = static_cast<double>( x - cibleX );
   const double dy = static_cast<double>( y - cibleY );
   const double orientationCible = std::atan2( -dy, dx );
   const double diff = normaliseAngle( orientationCible - orientation );

   setOrientation( orientation + diff * intensite );

}


void Bestiole::demiTour( void )
{

   ajusterOrientation( PI );

}


double Bestiole::borneVitesse( double value )
{

   return std::max( 0.0, std::min( value, MAX_VITESSE ) );

}


void Bestiole::setVitesse( double nouvelleVitesse )
{

   vitesse = borneVitesse( nouvelleVitesse );

}


void Bestiole::resetVitesse( void )
{

   vitesse = getCruiseSpeed();

}


double Bestiole::distance( const Bestiole & b ) const
{

   const double dx = static_cast<double>( x - b.x );
   const double dy = static_cast<double>( y - b.y );
   return std::sqrt( dx * dx + dy * dy );

}


void Bestiole::setComportement( std::unique_ptr<IBehavior> nouveauComportement )
{

   if ( nouveauComportement )
      comportement = std::move( nouveauComportement );
   else
      comportement = std::make_unique<GregariousBehavior>();

}


void Bestiole::addSensor( std::unique_ptr<ISensor> sensor )
{

   if ( sensor )
      sensors.push_back( std::move( sensor ) );

}


void Bestiole::addAccessory( std::unique_ptr<IAccessory> accessory )
{

   if ( accessory )
      accessories.push_back( std::move( accessory ) );

}


double Bestiole::speedMultiplier( void ) const
{

   double value = 1.0;
   for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = accessories.begin(); it != accessories.end(); ++it )
      value *= ( *it )->speedMultiplier();
   return value;

}


double Bestiole::speedDivisor( void ) const
{

   double value = 1.0;
   for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = accessories.begin(); it != accessories.end(); ++it )
      value *= ( *it )->speedDivisor();
   return value;

}


double Bestiole::getCruiseSpeed( void ) const
{

   const double adjusted = vitesseBase * speedMultiplier() / speedDivisor();
   return borneVitesse( adjusted );

}


double Bestiole::getDeathResistance( void ) const
{

   double value = 1.0;
   for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = accessories.begin(); it != accessories.end(); ++it )
      value *= ( *it )->deathResistanceMultiplier();
   return value;

}


double Bestiole::getCamouflageLevel( void ) const
{

   double value = 0.0;
   for ( std::vector<std::unique_ptr<IAccessory>>::const_iterator it = accessories.begin(); it != accessories.end(); ++it )
      value = std::max( value, ( *it )->camouflageLevel() );
   return value;

}
