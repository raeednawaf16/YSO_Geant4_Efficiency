#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
	RunAction *runAction = new RunAction();
	SetUserAction(runAction);

}

void ActionInitialization::Build() const
{
	PrimaryGeneratorAction *generator = new PrimaryGeneratorAction();
	SetUserAction(generator);

	SteppingAction *steppingAction = new SteppingAction(/*eventAction*/);
	SetUserAction(steppingAction);
	
	RunAction *runAction = new RunAction();
	SetUserAction(new RunAction());

	EventAction *eventAction = new EventAction(runAction);
	SetUserAction(eventAction);

}	
