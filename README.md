# EntitySystem
## Managers
### EntityManager
The EntityManager is the context in which Entities interact, and where they are stored.
Entities are stored as a group of Units, where each type of Unit is stored contiguously in memory in UnitManagers.
It keeps track of and assigns entityIds, and provides the interface between Units, Systems and the rest of the game engine.

### UnitManager
UnitManagers store one UnitType each in a sorted vector on the units enityId.

Instead of Units being inserted and erased during updates, they are inserted through an insertion queue, which is emptied at the beginning every update, and erased by setting a flag, where every flagged Unit is erased together after every update.
This is done to avoid iterator invalidation, and excessive reallocation.

## Units
### Units
Units are commonality between Events and Components.

Units make up Entities. Which Entity the Unit is owned by, is indicated by its entityId. 
An Entity can have any number of Units of any type.

Units have two ways of being "disabled". 
They can be set to be erased, in which case the Unit will be erased at the end of the current update. Units that are set to be erased on creation, will stay around for one update.
They can also be set to be ignored. An ignored Unit will with immediate effect, be ignored by Systems and erased at the end of the current update.

LimitedLifetimeUnits are Units with a lifetime that is automatically decreased every update by deltatime.
Once a LimitedLifetimeUnits lifetime is less than or equal to zero it is erased.

### Components
Components are a type of Unit.
Components are the basic building blocks of an entity.
Components are only erased if done so manually.

### Events
Events are a type of LimitedLifetimeUnit, and are erased automatically once lifetime is less than or equal to zero.

Basic Events do not have a target or an Entity its a part of, and its entityId is there only to uniquely identify the Event.

Targeted Events do have a target or Entity its a part of, and are only considered by Systems in context with other units with same entityId.

## Systems
Systems define interaction between Entities and Events.
They are updated in two parts. One update to change internal state of the System, and one update per Entity that has all the requested Units.
