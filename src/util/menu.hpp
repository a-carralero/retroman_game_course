#pragma once
#include <iostream>
#include "man/state_base.hpp"
#include "man/state_machine.hpp"
#include "man/game.hpp"

struct Options: StateBase
{
   void update() override{
      std::cout << "OPTIONS\n"
                << "Option 1\n"
                << "Option 2\n"
                << "Option 3\n"
                << "Press any Key to Scape\n";
      int key;
      std::cin >> key;
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      alive = false;
   }
};


struct Menu: StateBase
{
   Menu(StateMachine& sm) : state_machine(sm) {}

   void update() override{
      std::cout << "MENU\n"
                << "1) PLAY GAME\n"
                << "2) OPTIONS\n"
                << "3) ESCAPE\n";    
      int opt;
      std::cin >> opt;
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      switch (opt)
      {
         case 1: state_machine.addState<Game>(); break;
         case 2: state_machine.addState<Options>(); break;
         default: alive = false; break;
      }
   }
private:
   StateMachine& state_machine;
};