#pragma once

struct StateBase
{
 protected:
   bool alive = true;
 public:
   virtual void update() = 0;
   bool isAlive() {return alive; }
   virtual ~StateBase() = default;
};

