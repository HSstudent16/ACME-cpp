#include "../lib/vecN.hpp"
#include <string>
#include <math.h>
#include <iostream>
#include <map>

#define ACME_NULL_TYPE_2D           0x00
#define ACME_NULL_TYPE_3D           0x00

#define ACME_TILE_TYPE_2D           0x10
#define ACME_CONTACT_TYPE_2D        0x11
#define ACME_ENTITY_TYPE_2D         0x12
#define ACME_TILE_ENTITY_TYPE_2D    0x13
#define ACME_PHYSICS_ENTITY_TYPE_2D 0x14
#define ACME_CAMERA_TYPE_2D         0x15

#define ACME_TILE_TYPE_3D           0x20
#define ACME_CONTACT_TYPE_3D        0x21
#define ACME_ENTITY_TYPE_3D         0x22
#define ACME_TILE_ENTITY_TYPE_3D    0x23
#define ACME_PHYSICS_ENTITY_TYPE_3D 0x24
#define ACME_CAMERA_TYPE_3D         0x25

namespace ACME {
  struct TilemapIndex {
    unsigned short x;
    unsigned short y;
    unsigned short z;
    unsigned long hash;
  };
  struct EntityFlags {
    bool& operator[] (size_t idx) {
      switch (idx) {
        case 0: return onGround;
        case 1: return mayJump;
        case 2: return falling;
        case 3: return mayFly;
        case 4: return climbing;
        case 5: return inWater;
        case 6: return noGravity;
        case 7: return noClip;
        default: throw std::runtime_error("Entity Flag index " + std::to_string(idx) + " is out of range [0, 7]");
      }
    }
    const bool& operator[] (size_t idx) const {
      switch (idx) {
        case 0: return onGround;
        case 1: return mayJump;
        case 2: return falling;
        case 3: return mayFly;
        case 4: return climbing;
        case 5: return inWater;
        case 6: return noGravity;
        case 7: return noClip;
        default: throw std::runtime_error("Entity Flag index " + std::to_string(idx) + " is out of range [0, 7]");
      }
    }
    bool onGround  = false;
    bool mayJump   = false;
    bool falling   = false;
    bool mayFly    = false;
    bool climbing  = false;
    bool inWater   = false;
    bool noGravity = false;
    bool noClip    = false;
  };

  unsigned long getHash (unsigned short x, unsigned short y, unsigned short z);
  TilemapIndex unHash (unsigned long hash);

  TilemapIndex getTilemapIndex (unsigned short x, unsigned short y);
  TilemapIndex getTilemapIndex (unsigned short x, unsigned short y, unsigned short z);

  class Scene2D;
  class Scene3D;


  class Contact2D {
    public:
      Contact2D ();
      Contact2D (double x, double y);
      Contact2D (std::Vector2D pos);

      std::Vector2D position;
      std::Vector2D velocity = {0.0, 0.0};
      std::Vector2D acceleration = {0.0, 0.0};

      std::Vector2D size = {1.0, 1.0};
      std::Vector2D deltaSize = {0.0, 0.0};

      double mass = 1.0;

      int entityType = ACME_CONTACT_TYPE_2D;

      std::string typeName = "null";

  };
  class Contact3D {
    public:
      Contact3D ();
      Contact3D (double x, double y, double z);
      Contact3D (std::Vector3D pos);

      std::Vector3D position;
      std::Vector3D velocity = {0.0, 0.0, 0.0};
      std::Vector3D acceleration = {0.0, 0.0, 0.0};

      std::Vector3D size = {1.0, 1.0, 1.0};
      std::Vector3D deltaSize = {0.0, 0.0, 0.0};

      double mass = 1.0;

      int entityType = ACME_CONTACT_TYPE_3D;

      std::string type = "null";
  };

  struct TileConfig2D {
    const int entityType = ACME_TILE_TYPE_2D;
    std::string typeName;
    std::string name;

    char alias;

    std::Vector2D initialSize = {1.0, 1.0};
    std::Vector2D initialOffset = {0.0, 0.0};
  };
  struct TileConfig3D {
    const int entityType = ACME_TILE_TYPE_3D;
    std::string typeName;
    std::string name;

    char alias;

    std::Vector3D initialSize = {1.0, 1.0, 1.0};
    std::Vector3D initialOffset = {0.0, 0.0, 0.0};
  };


  class ContactList2D {
    public:
      unsigned long add (Contact2D* entry);
      unsigned long add (Contact2D* entry, unsigned long index);
      unsigned long remove ();
      unsigned long remove (unsigned long index);

      Contact2D* item (unsigned long index);

      unsigned long size ();

    private:
      unsigned long length = 0;
      Contact2D *list = new Contact2D[0];
  };
  class ContactList3D {
    public:
      virtual unsigned long add (Contact3D* entry);
      virtual unsigned long add (Contact3D* entry, unsigned long index);
      virtual unsigned long remove ();
      virtual unsigned long remove (unsigned long index);

      virtual Contact3D* item (unsigned long index);

      virtual unsigned long size ();

    private:
      unsigned long length = 0;
      Contact3D *list = new Contact3D[0];
  };

  class Entity2D : public Contact2D {
    public:
      Entity2D ();
      Entity2D (double x, double y);
      Entity2D (std::Vector2D pos);

      Scene2D *parentScene;

      virtual void tick (double deltaTime);
      virtual void reset ();

      void (*move) (Entity2D *self, double deltaTime) = nullptr;
      void (*onreset) (Entity2D *self) = nullptr;

      std::Vector2D initialPosition;
      std::Vector2D initialSize = {1.0, 1.0};
      std::Vector2D facing;

      double health = 1.0;

      
      bool isDead = false;
      bool suspended = false;

      EntityFlags flags;

      unsigned long index = 0;

      std::string name = "";
  };
  class Entity3D : public Contact3D {
    public:
      Entity3D ();
      Entity3D (double x, double y, double z);
      Entity3D (std::Vector3D pos);

      Scene3D *parentScene;

      virtual void tick (double deltaTime);
      virtual void reset ();

      void (*move) (Entity3D *self, double deltaTime) = nullptr;
      void (*onreset) (Entity3D *self) = nullptr;

      std::Vector3D initialPosition;
      std::Vector3D initialSize = {1.0, 1.0, 1.0};
      std::Vector3D facing;

      double health = 1.0;

      bool isDead = false;
      bool suspended = false;

      EntityFlags flags;

      unsigned long index = 0;

      std::string name = "";
  };

  class EntityList2D {
    public:
      unsigned long add (Entity2D* entry);
      unsigned long add (Entity2D* entry, unsigned long index);
      unsigned long remove ();
      unsigned long remove (unsigned long index);

      void tickAll (double deltaTime);
      void wipeAll ();
      void wipeDead ();
      void resetAll ();

      Entity2D* item (unsigned long index);

      unsigned long size ();

    private:
      unsigned long length = 0;
      Entity2D *list = new Entity2D[0];
  };
  class EntityList3D {
    public:
      unsigned long add (Entity3D* entry);
      unsigned long add (Entity3D* entry, unsigned long index);
      unsigned long remove ();
      unsigned long remove (unsigned long index);

      void tickAll (double deltaTime);
      void wipeAll ();
      void wipeDead ();
      void resetAll ();

      Entity3D* item (unsigned long index);

      unsigned long size ();

    private:
      unsigned long length = 0;
      Entity3D *list = new Entity3D[0];
  };

  class TileEntity2D : public Entity2D {
    public:
      TileEntity2D ();
      TileEntity2D (double x, double y);
      TileEntity2D (std::Vector2D pos);

      void tick (double deltaTime);
      void reset ();
      void (*onstatechange)(TileEntity2D *self, unsigned short newState, unsigned short oldState) = nullptr;

      TilemapIndex index;

      unsigned short initialState = 0;
      unsigned short previousState = 0;
      unsigned short state = 0;
  };
  class TileEntity3D : public Entity3D {
    public:
      TileEntity3D ();
      TileEntity3D (double x, double y, double z);
      TileEntity3D (std::Vector3D pos);

      void tick (double deltaTime);
      void reset ();
      void (*onstatechange)(TileEntity3D *self, unsigned short new_state, unsigned short last_state) = nullptr;

      TilemapIndex index;

      unsigned short initialState = 0;
      unsigned short previousState = 0;
      unsigned short state = 0;
  };

  class PhysicsEntity2D : public Entity2D {
    public:
      PhysicsEntity2D ();
      PhysicsEntity2D (double x, double y);
      PhysicsEntity2D (std::Vector2D pos);

      std::Vector2D previousPosition = {0.0, 0.0};

      void tick (double deltaTime);
      void reset ();

      void (*oncollide)(PhysicsEntity2D *self, Contact2D* contact) = nullptr;
      void (*oncollidetop)(PhysicsEntity2D *self, Contact2D* contact) = nullptr;
      void (*oncollidebottom)(PhysicsEntity2D *self, Contact2D* contact) = nullptr;
      void (*oncollideeast)(PhysicsEntity2D *self, Contact2D* contact) = nullptr;
      void (*oncollidewest)(PhysicsEntity2D *self, Contact2D* contact) = nullptr;

      ContactList2D getContacts ();
    
    private:
      void getTilemapContacts (ContactList2D* list);
      void getEntityContacts (ContactList2D* list);
  };
  class PhysicsEntity3D : public Entity3D {
    public:
      PhysicsEntity3D ();
      PhysicsEntity3D (double x, double y, double z);
      PhysicsEntity3D (std::Vector3D pos);

      std::Vector3D previousPosition = {0.0, 0.0, 0.0};

      void tick (double deltaTime);
      void reset ();

      void (*oncollide)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
      void (*oncollidetop)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
      void (*oncollidebottom)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
      void (*oncollideeast)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
      void (*oncollidewest)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
      void (*oncollidenorth)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
      void (*oncollidesouth)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;

      ContactList3D getContacts ();

    private:
      void getTilemapContacts (ContactList3D* list);
      void getEntityContacts (ContactList3D* list);
  };

  struct EntityConfig2D {
    int entityType = ACME_ENTITY_TYPE_2D;
    std::string typeName;
    std::string name;

    std::Vector2D initialSize = {1.0, 1.0};
    std::Vector2D initialOffset = {0.0, 0.0};
    
    void (*move) (Entity2D* self, double deltaTime) = nullptr;
    void (*blit) (Contact2D* tileOrEntity) = nullptr;
    void (*onreset) (Entity2D* self) = nullptr;
    void (*onstatechange)(TileEntity2D* self, unsigned short newState, unsigned short oldState) = nullptr;
    void (*oncollide)(PhysicsEntity2D* self, Contact2D* contact) = nullptr;
    void (*oncollidetop)(PhysicsEntity2D* self, Contact2D* contact) = nullptr;
    void (*oncollidebottom)(PhysicsEntity2D* self, Contact2D* contact) = nullptr;
    void (*oncollideeast)(PhysicsEntity2D* self, Contact2D* contact) = nullptr;
    void (*oncollidewest)(PhysicsEntity2D* self, Contact2D* contact) = nullptr;
  };
  struct EntityConfig3D {
    int entityType = ACME_ENTITY_TYPE_3D;
    std::string typeName;
    std::string name;

    std::Vector3D initialSize = {1.0, 1.0, 1.0};
    std::Vector3D initialOffset = {0.0, 0.0, 0.0};
    
    void (*move) (Entity3D *self, double deltaTime) = nullptr;
    void (*onreset) (Entity3D *self) = nullptr;
    void (*onstatechange)(TileEntity3D *self, unsigned short newState, unsigned short oldState) = nullptr;
    void (*oncollide)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
    void (*oncollidetop)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
    void (*oncollidebottom)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
    void (*oncollideeast)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
    void (*oncollidewest)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
    void (*oncollidenorth)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
    void (*oncollidesouth)(PhysicsEntity3D *self, Contact3D* contact) = nullptr;
  };

  class Camera2D : public Entity2D {
    public:
      Camera2D ();

      void tick (double deltaTime);
      
      void (*blit)(Contact2D* tileOrEntity) = nullptr;

      Contact2D *target = nullptr;
      
      bool blitEntities = true;
      bool blitTiles    = true;

    private:
      void getTilemapContacts (ContactList2D* list);
      void getEntityContacts (ContactList2D* list);
  };
  class Camera3D : public Entity3D {
    public:
      Camera3D ();

      Contact3D *target = nullptr;
  };

  class Tilemap2D {
    public:
      Tilemap2D (unsigned short $width, unsigned short $height);
      Tilemap2D (unsigned short $width, unsigned short $height, unsigned short *$data);
      ~Tilemap2D ();

      unsigned short read (TilemapIndex idx);
      unsigned short write (TilemapIndex idx, unsigned short value);

      unsigned short *data;
      unsigned short width;
      unsigned short height;
    
    private:
      bool isDataInternal;
  };
  class Tilemap3D {
    public:
      Tilemap3D (unsigned short $width, unsigned short $height, unsigned short $depth);
      Tilemap3D (unsigned short $width, unsigned short $height, unsigned short $depth, unsigned short *$data);
      ~Tilemap3D ();

      unsigned short read (TilemapIndex idx);
      unsigned short write (TilemapIndex idx, unsigned short value);

      unsigned short *data;
      unsigned short width;
      unsigned short height;
      unsigned short depth;

    private:
      bool isDataInternal;
  };

  
  class Scene2D {
    public:
      Scene2D() {}

      int debug ();
      void tick (double deltaTime);
      void wipe ();
      void reset ();
      void loadMap (Tilemap2D *map);
      void reloadMap ();

      void summon (EntityConfig2D config, double x, double y);
      void summon (EntityConfig2D config, std::Vector2D pos);
      void kill (Entity2D *entity);
      void link (Entity2D *entity);
      void unlink (Entity2D *entity);

      void registerTileConfig (TileConfig2D config);

      void (*ontick) (double deltaTime) = nullptr;
      void (*onreset) () = nullptr;
      void (*onwipe) () = nullptr;
      void (*onmapchange) () = nullptr;

      unsigned long tick_count = 0;

      bool voidWorld = false;

      Tilemap2D *tilemap = nullptr;
      EntityList2D entities;
      EntityList2D tileEntities;
    
    private:
      Tilemap2D *newMap = nullptr;
      EntityList2D summonedEntities;
      std::map<char, TileConfig2D> tileConfigs;

      bool shouldWipe = false;
      bool shouldReset = false;
  };
  class Scene3D {
    public:
      Scene3D() {}

      void debug ();
      void tick (double deltaTime);
      void wipe ();
      void reset ();
      void loadMap (Tilemap3D *map);
      void reloadMap ();

      void summon (EntityConfig3D rules, double x, double y, double z);
      void summon (EntityConfig3D rules, std::Vector3D pos);
      void kill (Entity3D *entity);
      void link (Entity3D *entity);
      void unlink (Entity3D *entity);

      void (*ontick) (double deltaTime) = nullptr;
      void (*onreset) () = nullptr;
      void (*onwipe) () = nullptr;
      void (*onmapchange) () = nullptr;

      unsigned long tick_count = 0;

      bool voidWorld = false;

      Tilemap3D *tilemap = nullptr;
      EntityList3D entities;
      EntityList3D tileEntities;
    
    private:
      Tilemap3D *newMap = nullptr;
      bool shouldWipe = false;
      bool shouldReset = false;
  };
  
};
