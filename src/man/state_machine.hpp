#pragma once
#include <stack>
#include <memory>
#include <type_traits>
#include "man/state_base.hpp"


class StateMachine
{
   std::stack<std::unique_ptr<StateBase>> state_machine;
   bool alive = true;
public:

   template <typename State_t, typename... Ts>
   void addState(Ts&&... args)
   {
      static_assert(
         std::is_base_of<StateBase, State_t>::value, 
         "Error invalid type State_t\n"
      );

      state_machine.push(
         std::make_unique<State_t>(std::forward<Ts>(args)...)
      );
   }

   void update()
   {
      StateBase& s = *(state_machine.top());
      if (s.isAlive()){
         s.update();
      }
      else{
         state_machine.pop();
      }
   }

   bool isAlive() {return !state_machine.empty(); }
};