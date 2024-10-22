#include "acme.hpp";

#define callIfAble(a)  if (a != nullptr) a
#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

bool aabb (ACME::Contact2D* a, ACME::Contact2D* b) {
  return (
    a->position.x + a->size.x > b->position.x &&
    a->position.y + a->size.y > b->position.y &&
    b->position.x + b->size.x > a->position.x &&
    b->position.y + b->size.y > a->position.y
  );
}
bool aabb (ACME::Contact3D* a, ACME::Contact3D* b) {
  return (
    a->position.x + a->size.x > b->position.x &&
    a->position.y + a->size.y > b->position.y &&
    a->position.z + a->size.z > b->position.z &&
    b->position.x + b->size.x > a->position.x &&
    b->position.y + b->size.y > a->position.y &&
    b->position.z + b->size.z > a->position.z
  );
}

namespace ACME {

  //
  // Tilemap coordinate hash functions
  //

  unsigned long getHash (unsigned short x, unsigned short y, unsigned short z) {
    return (z << 32) | (y << 16) | x;
  }
  TilemapIndex unHash (unsigned long hash) {
    TilemapIndex idx;
    idx.hash = hash;
    idx.x = hash & 0xFFFF;
    idx.y = (hash >> 0x10) & 0xFFFF;
    idx.z = (hash >> 0x20) & 0xFFFF;

    return idx;
  }


  //
  // Tilemap index generators
  //

  TilemapIndex getTilemapIndex (unsigned short x, unsigned short y) {
    TilemapIndex idx;
    idx.hash = getHash(x, y, 0);
    idx.x = x;
    idx.y = y;
    idx.z = 0;

    return idx;
  }
  TilemapIndex getTilemapIndex (unsigned short x, unsigned short y, unsigned short z) {
    TilemapIndex idx;
    idx.hash = getHash(x, y, z);
    idx.x = x;
    idx.y = y;
    idx.z = z;

    return idx;
  }


  //
  // Parent class for all ACME components
  //

  Contact2D::Contact2D () {
    position = {0.0, 0.0};
  }
  Contact2D::Contact2D (double x, double y) {
    position = {x, y};
  }
  Contact2D::Contact2D (std::Vector2D pos) {
    position = pos;
  }

  Contact3D::Contact3D () {
    position = {0.0, 0.0, 0.0};
  }
  Contact3D::Contact3D (double x, double y, double z) {
    position = {x, y, z};
  }
  Contact3D::Contact3D (std::Vector3D pos) {
    position = pos;
  }


  //
  // Dynamic list specifically for contacts & derivatives
  //

  unsigned long ContactList2D::add (Contact2D* item) {
    Contact2D *newlist = new Contact2D[length + 1];

    for (unsigned long i = 0; i < length; i++)
      newlist[i] = list[i];

    newlist[length] = *item;

    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList2D::add (Contact2D* item, unsigned long index) {
    Contact2D *newlist = new Contact2D[length + 1];
    unsigned long i;


    for (i = 0; i < index; i++)
      newlist[i] = list[i];

    newlist[index] = *item;

    for (; i <= length; i++)
      newlist[i] = list[i-1];
    
    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList2D::remove () {
    Contact2D *newlist = new Contact2D[length - 1];

    for (unsigned long i = 0; i < length-1; i++)
      newlist[i] = list[i];

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList2D::remove (unsigned long index) {
    Contact2D *newlist = new Contact2D[length - 1];

    for (unsigned long i = 0, j = 0; i < length; i++) {
      if (i == index) continue;
      newlist[j] = list[i];
      j++;
    }

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList2D::size () {
    return length;
  }
  Contact2D* ContactList2D::item (unsigned long index) {
    return &list[index];
  }

  unsigned long ContactList3D::add (Contact3D* item) {
    Contact3D *newlist = new Contact3D[length + 1];

    for (unsigned long i = 0; i < length; i++)
      newlist[i] = list[i];

    newlist[length] = *item;

    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList3D::add (Contact3D* item, unsigned long index) {
    Contact3D *newlist = new Contact3D[length + 1];
    unsigned long i;


    for (i = 0; i < index; i++)
      newlist[i] = list[i];

    newlist[index] = *item;

    for (; i <= length; i++)
      newlist[i] = list[i-1];
    
    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList3D::remove () {
    Contact3D *newlist = new Contact3D[length - 1];

    for (unsigned long i = 0; i < length-1; i++)
      newlist[i] = list[i];

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList3D::remove (unsigned long index) {
    Contact3D *newlist = new Contact3D[length - 1];

    for (unsigned long i = 0, j = 0; i < length; i++) {
      if (i == index) continue;
      newlist[j] = list[i];
      j++;
    }

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long ContactList3D::size () {
    return length;
  }
  Contact3D* ContactList3D::item (unsigned long index) {
    return &list[index];
  }


  //
  // Entity class; moving components that don't interact with the world
  // (e.g. text display)
  //

  Entity2D::Entity2D () : Contact2D () {
    initialPosition = {0.0, 0.0};
    entityType = ACME_ENTITY_TYPE_2D;
  }
  Entity2D::Entity2D (double x, double y) : Contact2D (x, y) {
    initialPosition = {x, y};
    entityType = ACME_ENTITY_TYPE_2D;
  }
  Entity2D::Entity2D (std::Vector2D pos) : Contact2D(pos) {
    initialPosition = pos;
    entityType = ACME_ENTITY_TYPE_2D;
  }

  void Entity2D::tick (double deltaTime) {
    acceleration = {0.0, 0.0};

    callIfAble(move)(this, deltaTime);

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;    
  }
  void Entity2D::reset () {
    position = initialPosition;
    velocity = {0.0, 0.0};
    acceleration = {0.0, 0.0};
    health = 1.0;
    isDead = false;
    size = initialSize;
    deltaSize = {0.0, 0.0};
  }

  Entity3D::Entity3D () : Contact3D () {
    initialPosition = {0.0, 0.0, 0.0};
    entityType = ACME_ENTITY_TYPE_3D;
  }
  Entity3D::Entity3D (double x, double y, double z) : Contact3D (x, y, z) {
    initialPosition = {x, y, z};
    entityType = ACME_ENTITY_TYPE_3D;
  }
  Entity3D::Entity3D (std::Vector3D pos) : Contact3D(pos) {
    initialPosition = pos;
    entityType = ACME_ENTITY_TYPE_3D;
  }

  void Entity3D::tick (double deltaTime) {
    acceleration = {0.0, 0.0, 0.0};

    callIfAble(move)(this, deltaTime);

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

  }
  void Entity3D::reset () {
    position = initialPosition;
    velocity = {0.0, 0.0, 0.0};
    acceleration = {0.0, 0.0, 0.0};
    health = 1.0;
    isDead = false;
    size = initialSize;
    deltaSize = {0.0, 0.0, 0.0};
  }


  //
  // Dynamic list specifically for contacts & derivatives
  //

  unsigned long EntityList2D::add (Entity2D* item) {
    Entity2D *newlist = new Entity2D[length + 1];

    for (unsigned long i = 0; i < length; i++)
      newlist[i] = list[i];

    newlist[length] = *item;

    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList2D::add (Entity2D* item, unsigned long index) {
    Entity2D *newlist = new Entity2D[length + 1];
    unsigned long i;


    for (i = 0; i < index; i++)
      newlist[i] = list[i];

    newlist[index] = *item;

    for (; i <= length; i++)
      newlist[i] = list[i-1];
    
    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList2D::remove () {
    Entity2D *newlist = new Entity2D[length - 1];

    for (unsigned long i = 0; i < length-1; i++)
      newlist[i] = list[i];

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList2D::remove (unsigned long index) {
    Entity2D *newlist = new Entity2D[length - 1];

    for (unsigned long i = 0, j = 0; i < length; i++) {
      if (i == index) continue;
      newlist[j] = list[i];
      j++;
    }

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList2D::size () {
    return length;
  }
  Entity2D* EntityList2D::item (unsigned long index) {
    return &list[index];
  }
  void EntityList2D::tickAll (double deltaTime) {
    for (unsigned long i = 0; i < length; i++)
      list[i].tick(deltaTime);    
  }
  void EntityList2D::resetAll () {
    for (unsigned long i = 0; i < length; i++)
      list[i].reset();
  }
  void EntityList2D::wipeAll () {
    Entity2D *newList = new Entity2D[0];
    length = 0;
    delete[] list;
    list = newList;
  }
  void EntityList2D::wipeDead () {
    unsigned long newLength = 0, i, j;
    Entity2D* entity;

    for (i = 0; i < length; i++) {
      entity = &list[i];

      if (entity->isDead) continue;

      newLength ++;
    }

    if (newLength == length) return;
    if (newLength == 0) return wipeAll();

    Entity2D *newList = new Entity2D[newLength];

    for (i = 0, j = 0; i < length; i++) {
      entity = &list[i];
      if (entity->isDead) continue;
      newList[j] = list[i];
      j++;
    }

    length = newLength;
    delete[] list;
    list = newList;
  }

  unsigned long EntityList3D::add (Entity3D* item) {
    Entity3D *newlist = new Entity3D[length + 1];

    for (unsigned long i = 0; i < length; i++)
      newlist[i] = list[i];

    newlist[length] = *item;

    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList3D::add (Entity3D* item, unsigned long index) {
    Entity3D *newlist = new Entity3D[length + 1];
    unsigned long i;


    for (i = 0; i < index; i++)
      newlist[i] = list[i];

    newlist[index] = *item;

    for (; i <= length; i++)
      newlist[i] = list[i-1];
    
    length ++;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList3D::remove () {
    Entity3D *newlist = new Entity3D[length - 1];

    for (unsigned long i = 0; i < length-1; i++)
      newlist[i] = list[i];

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList3D::remove (unsigned long index) {
    Entity3D *newlist = new Entity3D[length - 1];

    for (unsigned long i = 0, j = 0; i < length; i++) {
      if (i == index) continue;
      newlist[j] = list[i];
      j++;
    }

    length --;
    delete[] list;
    list = newlist;

    return length;
  }
  unsigned long EntityList3D::size () {
    return length;
  }
  Entity3D* EntityList3D::item (unsigned long index) {
    return &list[index];
  }
  void EntityList3D::tickAll (double deltaTime) {
    for (unsigned long i = 0; i < length; i++)
      list[i].tick(deltaTime);
  }
  void EntityList3D::resetAll () {
    for (unsigned long i = 0; i < length; i++)
      list[i].reset();
  }
  void EntityList3D::wipeAll () {
    Entity3D *newList = new Entity3D[0];
    length = 0;
    delete[] list;
    list = newList;
  }
  void EntityList3D::wipeDead () {
    unsigned long newLength = 0, i, j;
    Entity3D* entity;

    for (i = 0; i < length; i++) {
      entity = &list[i];

      if (entity->isDead) continue;

      newLength ++;
    }

    if (newLength == length) return;
    if (newLength == 0) return wipeAll();

    Entity3D *newList = new Entity3D[newLength];

    for (i = 0, j = 0; i < length; i++) {
      entity = &list[i];
      if (entity->isDead) continue;
      newList[j] = list[i];
      j++;
    }

    length = newLength;
    delete[] list;
    list = newList;
  }


  //
  // Tile Entity class; grid-locked immovable components that don't interact with the world
  // but are linked to a specific tile (e.g. chest)
  //

  TileEntity2D::TileEntity2D () : Entity2D () {
    index = getTilemapIndex(0, 0);
    entityType = ACME_TILE_ENTITY_TYPE_2D;
  }
  TileEntity2D::TileEntity2D (double x, double y) : Entity2D (floor(x), floor(y)) {
    index = getTilemapIndex(floor(x), floor(y));
    entityType = ACME_TILE_ENTITY_TYPE_2D;
  }
  TileEntity2D::TileEntity2D (std::Vector2D pos) : Entity2D (floor(pos.x), floor(pos.y)) {
    index = getTilemapIndex(floor(pos.x), floor(pos.y));
    entityType = ACME_TILE_ENTITY_TYPE_2D;
  }

  void TileEntity2D::tick (double deltaTime) {
    if (state != previousState) {
      callIfAble(onstatechange)(this, state, previousState);
      previousState = state;
    }
  }
  void TileEntity2D::reset () {
    position = initialPosition;
    velocity = {0.0, 0.0};
    acceleration = {0.0, 0.0};
    health = 1.0;
    isDead = false;
    size = initialSize;
    deltaSize = {0.0, 0.0};
    state = initialState;
    previousState = initialState;

    callIfAble(onreset)(this);
  }

  TileEntity3D::TileEntity3D () : Entity3D () {
    index = getTilemapIndex(0, 0, 0);
    entityType = ACME_TILE_ENTITY_TYPE_3D;
  }
  TileEntity3D::TileEntity3D (double x, double y, double z) : Entity3D (floor(x), floor(y), floor(z)) {
    index = getTilemapIndex (floor(x), floor(y), floor(z));
    entityType = ACME_TILE_ENTITY_TYPE_3D;
  }
  TileEntity3D::TileEntity3D (std::Vector3D pos) : Entity3D (floor(pos.x), floor(pos.y), floor(pos.z)) {
    index = getTilemapIndex (floor(pos.x), floor(pos.y), floor(pos.z));
    entityType = ACME_TILE_ENTITY_TYPE_3D;
  } 

  void TileEntity3D::tick (double deltaTime) {
    if (state != previousState) {
      callIfAble(onstatechange)(this, state, previousState);
      previousState = state;
    }
  }
  void TileEntity3D::reset () {
    position = initialPosition;
    velocity = {0.0, 0.0, 0.0};
    acceleration = {0.0, 0.0, 0.0};
    health = 1.0;
    isDead = false;
    size = initialSize;
    deltaSize = {0.0, 0.0, 0.0};
    state = initialState;
    previousState = initialState;

    callIfAble(onreset)(this);
  }


  //
  // Physics Entity class; moving components that interact with and/or collide with the world and
  // other components (e.g. player)
  //
  
  PhysicsEntity2D::PhysicsEntity2D () : Entity2D () {
    entityType = ACME_PHYSICS_ENTITY_TYPE_2D;
  }
  PhysicsEntity2D::PhysicsEntity2D (double x, double y) : Entity2D (x, y) {
    entityType = ACME_PHYSICS_ENTITY_TYPE_2D;
  }
  PhysicsEntity2D::PhysicsEntity2D (std::Vector2D pos) : Entity2D (pos) {
    entityType = ACME_PHYSICS_ENTITY_TYPE_2D;
  }

  void PhysicsEntity2D::tick (double deltaTime) {
    acceleration = {0.0, 0.0};

    callIfAble(move)(this, deltaTime);
    
    previousPosition = position;

    std::Vector2D tvel = velocity + acceleration * deltaTime;

    position.x += tvel.x * deltaTime;

    ContactList2D contacts = getContacts ();

    
    for (unsigned long i = 0, len = contacts.size(); i < len; i++) {
      Contact2D* contact = contacts.item(i);

      if (aabb(contact, this)) {
        callIfAble(oncollide)(this, contact);

        if (tvel.x - contact->velocity.x > 0) {
          callIfAble(oncollideeast)(this, contact);
        } else {
          callIfAble(oncollidewest)(this, contact);
        }
      }
    }

    position.y += tvel.y * deltaTime;

    contacts = getContacts ();

    for (unsigned long i = 0, len = contacts.size(); i < len; i++) {
      Contact2D* contact = contacts.item(i);

      if (aabb(contact, this)) {
        callIfAble(oncollide)(this, contact);

        if (tvel.y - contact->velocity.y > 0) {
          callIfAble(oncollidebottom)(this, contact);
        } else {
          callIfAble(oncollidetop)(this, contact);
        }
      }
    }

    velocity = position - previousPosition;

  }
  void PhysicsEntity2D::reset () {
    position = initialPosition;
    previousPosition = position;
    velocity = {0.0, 0.0};
    acceleration = {0.0, 0.0};
    health = 1.0;
    isDead = false;
    size = initialSize;
    deltaSize = {0.0, 0.0};
  }
  ContactList2D PhysicsEntity2D::getContacts () {
    ContactList2D output;

    if (parentScene == nullptr || parentScene->voidWorld) {
      return output;
    }

    if (parentScene->tilemap != nullptr) getTilemapContacts(&output);

    getEntityContacts(&output);

    return output;
  }
  void PhysicsEntity2D::getTilemapContacts(ContactList2D *list) {
    Tilemap2D *map = parentScene->tilemap;

    unsigned short 
      data,
      ddx, ddy,
      startX = max(floor(position.x), 0),
      startY = max(floor(position.y), 0),
      endX = min(ceil(position.x + size.x), map->width),
      endY = min(ceil(position.y + size.y), map->height);

    TilemapIndex index;

    for (ddy = startY; ddy < endY; ddy ++) {
      for (ddx = startX; ddx < endX; ddx ++) {
        index = getTilemapIndex(ddx, ddy);

        data = map->read(index);

        Contact2D tile{ddx, ddy};

        tile.entityType = ACME_TILE_TYPE_2D;

        list->add(&tile);
      }
    }    
  }
  void PhysicsEntity2D::getEntityContacts(ContactList2D *list) {
    EntityList2D* all = &parentScene->entities;

    for (unsigned long i = 0, len = all->size(); i < len; i++) {
      Entity2D* entity = all->item(i);

      if (aabb(this, entity))
        list->add(entity);
      
    }
  }

  PhysicsEntity3D::PhysicsEntity3D () : Entity3D () {
    entityType = ACME_PHYSICS_ENTITY_TYPE_3D;
  }
  PhysicsEntity3D::PhysicsEntity3D (double x, double y, double z) : Entity3D (x, y, z) {
    entityType = ACME_PHYSICS_ENTITY_TYPE_3D;
  }
  PhysicsEntity3D::PhysicsEntity3D (std::Vector3D pos) : Entity3D (pos) {
    entityType = ACME_PHYSICS_ENTITY_TYPE_3D;
  }

  void PhysicsEntity3D::tick (double deltaTime) {
    acceleration = {0.0, 0.0};

    callIfAble(move)(this, deltaTime);
    
    previousPosition = position;

    std::Vector3D tvel = velocity + acceleration * deltaTime;

    position.x += tvel.x * deltaTime;

    ContactList3D contacts = getContacts ();

    
    for (unsigned long i = 0, len = contacts.size(); i < len; i++) {
      Contact3D* contact = contacts.item(i);

      if (aabb(contact, this)) {
        callIfAble(oncollide)(this, contact);

        if (tvel.x - contact->velocity.x > 0) {
          callIfAble(oncollideeast)(this, contact);
        } else {
          callIfAble(oncollidewest)(this, contact);
        }
      }
    }

    position.y += tvel.y * deltaTime;

    contacts = getContacts ();

    for (unsigned long i = 0, len = contacts.size(); i < len; i++) {
      Contact3D* contact = contacts.item(i);

      if (aabb(contact, this)) {
        callIfAble(oncollide)(this, contact);

        if (tvel.y - contact->velocity.y > 0) {
          callIfAble(oncollidebottom)(this, contact);
        } else {
          callIfAble(oncollidetop)(this, contact);
        }
      }
    }

    position.z += tvel.z * deltaTime;

    contacts = getContacts ();

    for (unsigned long i = 0, len = contacts.size(); i < len; i++) {
      Contact3D* contact = contacts.item(i);

      if (aabb(contact, this)) {
        callIfAble(oncollide)(this, contact);

        if (tvel.z - contact->velocity.z > 0) {
          callIfAble(oncollidenorth)(this, contact);
        } else {
          callIfAble(oncollidesouth)(this, contact);
        }
      }
    }

    velocity = position - previousPosition;

  }
  void PhysicsEntity3D::reset () {
    position = initialPosition;
    previousPosition = position;
    velocity = {0.0, 0.0, 0.0};
    acceleration = {0.0, 0.0, 0.0};
    health = 1.0;
    isDead = false;
    size = initialSize;
    deltaSize = {0.0, 0.0, 0.0};
  }
  ContactList3D PhysicsEntity3D::getContacts () {
    ContactList3D output;

    if (parentScene == nullptr || parentScene->voidWorld) {
      return output;
    }

    if (parentScene->tilemap != nullptr) getTilemapContacts(&output);

    getEntityContacts(&output);

    return output;
  }
  void PhysicsEntity3D::getTilemapContacts(ContactList3D *list) {
    Tilemap3D *map = parentScene->tilemap;

    unsigned short 
      data,
      ddx, ddy, ddz,
      startX = max(floor(position.x), 0),
      startY = max(floor(position.y), 0),
      startZ = max(floor(position.z), 0),
      endX = min(ceil(position.x + size.x), map->width),
      endY = min(ceil(position.y + size.y), map->height),
      endZ = min(ceil(position.z + size.z), map->depth);

    TilemapIndex index;

    for (ddy = startY; ddy < endY; ddy ++) {
      for (ddx = startX; ddx < endX; ddx ++) {
        index = getTilemapIndex(ddx, ddy);

        data = map->read(index);

        Contact3D tile{ddx, ddy, ddz};

        tile.entityType = ACME_TILE_TYPE_3D;

        list->add(&tile);
      }
    }    
  }
  void PhysicsEntity3D::getEntityContacts(ContactList3D *list) {
    EntityList3D* all = &parentScene->entities;

    for (unsigned long i = 0, len = all->size(); i < len; i++) {
      Entity3D* entity = all->item(i);

      if (aabb(this, entity))
        list->add(entity);
      
    }
  }


  //
  // Camera 2D class; moving component that uses collision detection to determine visible tiles & entites
  //

  Camera2D::Camera2D () : Entity2D () {
    entityType = ACME_CAMERA_TYPE_2D;
  }
  void Camera2D::tick (double deltaTime) {
    acceleration = {0.0, 0.0};

    callIfAble(move)(this, deltaTime);

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    ContactList2D contacts;

    if (blitTiles && parentScene->tilemap != nullptr)
      getTilemapContacts(&contacts);
    
    if (blitEntities)
      getEntityContacts(&contacts);

    for (unsigned long i = 0, l = contacts.size(); i < l; i++)
      blit(contacts.item(i));
    
  }
  void Camera2D::getTilemapContacts(ContactList2D *list) {
    Tilemap2D *map = parentScene->tilemap;

    unsigned short 
      data,
      ddx, ddy,
      startX = max(floor(position.x), 0),
      startY = max(floor(position.y), 0),
      endX = min(ceil(position.x + size.x), map->width),
      endY = min(ceil(position.y + size.y), map->height);

    TilemapIndex index;

    for (ddy = startY; ddy < endY; ddy ++) {
      for (ddx = startX; ddx < endX; ddx ++) {
        index = getTilemapIndex(ddx, ddy);

        data = map->read(index);

        Contact2D tile{ddx, ddy};

        list->add(&tile);
      }
    }    
  }
  void Camera2D::getEntityContacts(ContactList2D *list) {
    EntityList2D* all = &parentScene->entities;

    for (unsigned long i = 0, len = all->size(); i < len; i++) {
      Entity2D* entity = all->item(i);

      if (aabb(this, entity))
        list->add(entity);
      
    }
  }


  //
  // Camera 3D class; moving component that represent a 3D camera. Frustrum culling is not handled by this class
  //
  
  Camera3D::Camera3D () : Entity3D () {
    entityType = ACME_CAMERA_TYPE_3D;
  }


  //
  // Tilemap 2D class; a 2d integer bitmap for a texel-based scene
  //
  
  Tilemap2D::Tilemap2D (unsigned short $width, unsigned short $height) : width($width), height($height) {
    data = new unsigned short [width * height];
    isDataInternal = true;
  }
  Tilemap2D::Tilemap2D (unsigned short $width, unsigned short $height, unsigned short *$data) {
    width = $width;
    height = $height;
    data = $data;
    isDataInternal = false;
  }
  Tilemap2D::~Tilemap2D () {
    if (isDataInternal) delete[] data;
  }
  unsigned short Tilemap2D::read (TilemapIndex idx) {
    if (idx.x >= width || idx.y >= height || idx.z > 0)
      throw "[ACME] Unable to read from tilemap; invalid index.";
    return data[idx.hash];
  }
  unsigned short Tilemap2D::write (TilemapIndex idx, unsigned short value) {
    if (idx.x >= width || idx.y >= height || idx.z > 0)
      throw "[ACME] Unable to read from tilemap; invalid index.";
    
    unsigned short oldValue = data[idx.hash];

    data[idx.hash] = value;

    return oldValue;
  }


  //
  // Tilemap 3D class; a 3d integer bitmap for a voxel-based scene
  //

  Tilemap3D::Tilemap3D (unsigned short $width, unsigned short $height, unsigned short $depth) : width($width), height($height), depth($depth) {
    data = new unsigned short [width * height];
    isDataInternal = true;
  }
  Tilemap3D::Tilemap3D (unsigned short $width, unsigned short $height, unsigned short $depth, unsigned short *$data) {
    width = $width;
    height = $height;
    depth = $depth;
    data = $data;
    isDataInternal = false;
  }
  Tilemap3D::~Tilemap3D () {
    if (isDataInternal) delete[] data;
  }
  unsigned short Tilemap3D::read (TilemapIndex idx) {
    if (idx.x >= width || idx.y >= height || idx.z >= depth)
      throw "[ACME] Unable to read from tilemap; invalid index.";
    return data[idx.hash];
  }
  unsigned short Tilemap3D::write (TilemapIndex idx, unsigned short value) {
    if (idx.x >= width || idx.y >= height || idx.z >= depth)
      throw "[ACME] Unable to read from tilemap; invalid index.";
    
    unsigned short oldValue = data[idx.hash];

    data[idx.hash] = value;

    return oldValue;
  }


  //
  // Scene 2D class; a wrapper class for managing 2d entities & a tilemap
  //

  void Scene2D::tick (double deltaTime) {
    entities.tickAll (deltaTime);
    tileEntities.tickAll (deltaTime);

    callIfAble(ontick)(deltaTime);

    if (newMap != nullptr) {
      tilemap = newMap;
      newMap = nullptr;
      callIfAble(onmapchange)();
    }

    for (unsigned long i = summonedEntities.size(); i --> 0;) {
      Entity2D *entity = summonedEntities.item(i);

      if (entity->entityType == ACME_TILE_ENTITY_TYPE_2D) {
        tileEntities.add(entity);
      } else {
        entities.add(entity);
      }
      summonedEntities.remove(i);
    }

    if (shouldWipe) {
      entities.wipeAll();
      tileEntities.wipeAll();
      callIfAble(onwipe)();
      shouldWipe = false;
    }

    if (shouldReset) {
      entities.resetAll();
      tileEntities.resetAll();
      callIfAble(onreset)();
      shouldReset = false;
    }

    entities.wipeDead();
    
  }
  void Scene2D::wipe () {
    shouldWipe = true;
  }
  void Scene2D::reset () {
    shouldReset = true;
  }
  void Scene2D::loadMap (Tilemap2D *map) {
    newMap = map;
  }
  void Scene2D::reloadMap () {
    newMap = tilemap;
  }
  Entity2D Scene2D::summon (EntityConfig2D config, std::Vector2D pos) {
    switch (config.entityType) {
      case ACME_CAMERA_TYPE_2D:
        Camera2D *camera = new Camera2D();

        camera->position = pos + config.initialOffset;
        camera->initialPosition = pos + config.initialOffset;
        camera->size = config.initialSize;
        camera->initialSize = config.initialSize;

        camera->move = config.move;
        camera->onreset = config.onreset;
        camera->blit = config.blit;

        camera->typeName = config.typeName;
        camera->name = config.name;

        summonedEntities.add(camera);

        delete camera;        
      break;
      case ACME_CONTACT_TYPE_2D:
      case ACME_TILE_TYPE_2D:
        throw std::runtime_error("Cannot summon entity of type 'Contact2D'");
      break;
      case ACME_ENTITY_TYPE_2D:
        Entity2D *entity = new Entity2D (pos + config.initialOffset);

        entity->size = config.initialSize;
        entity->initialSize = config.initialSize;

        entity->move = config.move;
        entity->onreset = config.onreset;

        entity->typeName = config.typeName;
        entity->name = config.name;

        summonedEntities.add(entity);

        delete entity;
      break;
      case ACME_PHYSICS_ENTITY_TYPE_2D:
        PhysicsEntity2D *p_entity = new PhysicsEntity2D (pos + config.initialOffset);

        p_entity->size = config.initialSize;
        p_entity->initialSize = config.initialSize;

        p_entity->move = config.move;
        p_entity->onreset = config.onreset;
        p_entity->oncollide = config.oncollide;
        p_entity->oncollidebottom = config.oncollidebottom;
        p_entity->oncollideeast = config.oncollideeast;
        p_entity->oncollidewest = config.oncollidewest;
        p_entity->oncollidetop = config.oncollidetop;

        p_entity->typeName = config.typeName;
        p_entity->name = config.name;

        summonedEntities.add(p_entity);

        delete p_entity;
      break;
      case ACME_TILE_ENTITY_TYPE_2D:
        // Overwrite
      break;
      case ACME_NULL_TYPE_2D:
      break;

    }
  }
  Entity2D Scene2D::summon (EntityConfig2D config, double x, double y) {
    summon(config, std::vec2(x, y));
  }
  void Scene2D::kill (Entity2D *entity) {
    entity->isDead = true;
  }
  void Scene2D::link (Entity2D *entity) {}
  void Scene2D::unlink (Entity2D *entity) {}



  //
  // Scene 3D class; a wrapper class for manging 3d entities & a tilemap
  //

  void Scene3D::tick (double deltaTime) {
    entities.tickAll (deltaTime);
    tileEntities.tickAll (deltaTime);

    callIfAble(ontick)(deltaTime);

    if (newMap != nullptr) {
      tilemap = newMap;
      newMap = nullptr;
      callIfAble(onmapchange)();
    }

    if (shouldWipe) {
      entities.wipeAll();
      tileEntities.wipeAll();
      callIfAble(onwipe)();
      shouldWipe = false;
    }

    if (shouldReset) {
      entities.resetAll();
      tileEntities.resetAll();
      callIfAble(onreset)();
      shouldReset = false;
    }
  }
  void Scene3D::wipe () {
    shouldWipe = true;
  }
  void Scene3D::reset () {
    shouldReset = true;
  }
  void Scene3D::loadMap (Tilemap3D *map) {
    newMap = map;
  }
  void Scene3D::reloadMap () {
    newMap = tilemap;
  }


};
