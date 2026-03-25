#include "BestioleFactory.h"

#include "accessories/CamouflageAccessory.h"
#include "accessories/FinAccessory.h"
#include "accessories/ShellAccessory.h"
#include "behavior/CautiousBehavior.h"
#include "behavior/FearfulBehavior.h"
#include "behavior/GregariousBehavior.h"
#include "behavior/KamikazeBehavior.h"
#include "sensors/EarSensor.h"
#include "sensors/EyeSensor.h"

#include <array>
#include <utility>


BestioleFactory::BestioleFactory( const SimulationConfig & configuration )
   : config( configuration ), rng( std::random_device{}() )
{
}


BehaviorKind BestioleFactory::pickBehaviorKind( void )
{
   const std::array<double, 4> weights = {
      config.getGregariousPercent(),
      config.getFearfulPercent(),
      config.getKamikazePercent(),
      config.getCautiousPercent()
   };
   std::discrete_distribution<int> distribution( weights.begin(), weights.end() );

   switch ( distribution( rng ) )
   {
      case 0: return BehaviorKind::Gregarious;
      case 1: return BehaviorKind::Fearful;
      case 2: return BehaviorKind::Kamikaze;
      default: return BehaviorKind::Cautious;
   }
}


std::unique_ptr<Bestiole> BestioleFactory::createBestiole( BehaviorKind kind )
{
   // Factory centralizes behavior, sensors and accessories assembly.
   std::unique_ptr<Bestiole> b = std::make_unique<Bestiole>( createBehavior( kind ) );

   equipSensors( *b );
   equipAccessories( *b );
   b->resetVitesse();
   return b;
}


std::unique_ptr<Bestiole> BestioleFactory::createFromConfig( void )
{
   return createBestiole( pickBehaviorKind() );
}


std::unique_ptr<IBehavior> BestioleFactory::createBehaviorFromConfig( void )
{
   return createBehavior( pickBehaviorKind() );
}


std::vector<std::unique_ptr<Bestiole>> BestioleFactory::createInitialPopulation( void )
{
   std::vector<std::unique_ptr<Bestiole>> population;
   population.reserve( static_cast<size_t>( config.getInitialPopulation() ) );

   for ( int i = 0; i < config.getInitialPopulation(); ++i )
      population.push_back( createFromConfig() );

   return population;
}


std::unique_ptr<IBehavior> BestioleFactory::createBehavior( BehaviorKind kind ) const
{
   switch ( kind )
   {
      case BehaviorKind::Gregarious:
         return std::make_unique<GregariousBehavior>();
      case BehaviorKind::Fearful:
         return std::make_unique<FearfulBehavior>();
      case BehaviorKind::Kamikaze:
         return std::make_unique<KamikazeBehavior>();
      case BehaviorKind::Cautious:
      default:
         return std::make_unique<CautiousBehavior>();
   }
}


std::unique_ptr<Bestiole> BestioleFactory::tryCreateSpontaneousBirth( void )
{
   std::bernoulli_distribution spawnDist( config.getSpontaneousBirthRate() );
   if ( !spawnDist( rng ) )
      return nullptr;

   return createFromConfig();
}


std::unique_ptr<Bestiole> BestioleFactory::tryCreateClone( const Bestiole & parent )
{
   std::bernoulli_distribution cloneDist( config.getCloningRate() );
   if ( !cloneDist( rng ) )
      return nullptr;

   return std::make_unique<Bestiole>( parent );
}


void BestioleFactory::equipSensors( Bestiole & bestiole )
{
   std::bernoulli_distribution eyeDist( config.getEyeSensorRate() );
   std::bernoulli_distribution earDist( config.getEarSensorRate() );
   std::uniform_real_distribution<double> eyeAngleDist( 0.8, 1.6 );
   std::uniform_real_distribution<double> eyeRangeDist( 25.0, 55.0 );
   std::uniform_real_distribution<double> earRangeDist( 20.0, 60.0 );
   std::uniform_real_distribution<double> detectDist( 0.4, 1.0 );

   if ( eyeDist( rng ) )
      bestiole.addSensor( std::make_unique<EyeSensor>( eyeAngleDist( rng ), eyeRangeDist( rng ), detectDist( rng ) ) );

   if ( earDist( rng ) )
      bestiole.addSensor( std::make_unique<EarSensor>( earRangeDist( rng ), detectDist( rng ) ) );
}


void BestioleFactory::equipAccessories( Bestiole & bestiole )
{
   std::bernoulli_distribution finDist( config.getFinAccessoryRate() );
   std::bernoulli_distribution shellDist( config.getShellAccessoryRate() );
   std::bernoulli_distribution camoDist( config.getCamouflageAccessoryRate() );
   std::uniform_real_distribution<double> finCoeffDist( 1.0, 1.7 );
   std::uniform_real_distribution<double> shellPenaltyDist( 1.0, 1.6 );
   std::uniform_real_distribution<double> shellResistDist( 1.0, 2.4 );
   std::uniform_real_distribution<double> camoDistValue( 0.1, 0.9 );

   if ( finDist( rng ) )
      bestiole.addAccessory( std::make_unique<FinAccessory>( finCoeffDist( rng ) ) );

   if ( shellDist( rng ) )
      bestiole.addAccessory( std::make_unique<ShellAccessory>( shellPenaltyDist( rng ), shellResistDist( rng ) ) );

   if ( camoDist( rng ) )
      bestiole.addAccessory( std::make_unique<CamouflageAccessory>( camoDistValue( rng ) ) );
}
