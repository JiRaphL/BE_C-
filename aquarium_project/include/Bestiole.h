#ifndef _BESTIOLES_H_
#define _BESTIOLES_H_


#include "UImg.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;


class Milieu;
class IBehavior;
class ISensor;
class IAccessory;


class Bestiole
{

private :
   static const double     AFF_SIZE;
   static const double     MAX_VITESSE;
   static const double     LIMITE_VUE;

   static int              next;

private :
   int               identite;
   int               x, y;
   double            cumulX, cumulY;
   double            orientation;
   double            vitesse;
   double            vitesseBase;
   bool              alive;
   int               age;
   int               maxAge;

   T               * couleur;
   std::unique_ptr<IBehavior> comportement;
   std::vector<std::unique_ptr<ISensor>> sensors;
   std::vector<std::unique_ptr<IAccessory>> accessories;

private :
   void bouge( int xLim, int yLim );
   static double borneVitesse( double value );
   double speedMultiplier( void ) const;
   double speedDivisor( void ) const;

public :                                           // Forme canonique :
   Bestiole( void );                               // Constructeur par defaut
   Bestiole( std::unique_ptr<IBehavior> comportementInit );
   Bestiole( const Bestiole & b );                 // Constructeur de copies
   Bestiole & operator=( const Bestiole & b );
   ~Bestiole( void );                              // Destructeur
                                                   // Operateur d'affectation binaire par defaut
   void action( Milieu & monMilieu );
   void draw( UImg & support );

   bool jeTeVois( const Bestiole & b ) const;

   void initCoords( int xLim, int yLim );
   void setCoords( int nx, int ny );
   void avance( const Milieu & monMilieu );

   int getX( void ) const { return x; }
   int getY( void ) const { return y; }
   double getOrientation( void ) const { return orientation; }
   double getVitesse( void ) const { return vitesse; }
   double getVitesseBase( void ) const { return vitesseBase; }
   double getCruiseSpeed( void ) const;
   double getDeathResistance( void ) const;
   double getCamouflageLevel( void ) const;
   bool isAlive( void ) const { return alive; }
   int getAge( void ) const { return age; }
   int getMaxAge( void ) const { return maxAge; }
   std::size_t sensorCount( void ) const { return sensors.size(); }
   std::size_t accessoryCount( void ) const { return accessories.size(); }

   void setOrientation( double nouvelleOrientation );
   void ajusterOrientation( double deltaOrientation );
   void orienterVers( int cibleX, int cibleY, double intensite = 1.0 );
   void orienterOppose( int cibleX, int cibleY, double intensite = 1.0 );
   void demiTour( void );
   void setVitesse( double nouvelleVitesse );
   void resetVitesse( void );
   double distance( const Bestiole & b ) const;
   void kill( void ) { alive = false; }
   void revive( void ) { alive = true; }
   void setAge( int value ) { age = std::max( 0, value ); }
   void incrementAge( void ) { ++age; }
   void setMaxAge( int value ) { maxAge = std::max( 1, value ); }

   void setComportement( std::unique_ptr<IBehavior> nouveauComportement );
   const IBehavior * getComportement( void ) const { return comportement.get(); }
   void addSensor( std::unique_ptr<ISensor> sensor );
   void addAccessory( std::unique_ptr<IAccessory> accessory );

   friend bool operator==( const Bestiole & b1, const Bestiole & b2 );

};


#endif
