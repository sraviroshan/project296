/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */


#ifndef _CS296BASE_HPP_
#define _CS296BASE_HPP_

#include "render.hpp"
#include <Box2D/Box2D.h>
#include <cstdlib>

#define	RAND_LIMIT 32767

namespace cs296
{

  //! What is the difference between a class and a struct in C++?
  
  class base_sim_t;
  struct settings_t;
  
  //! Why do we use a typedef
  typedef base_sim_t* sim_create_fcn(); 

  //! Simulation settings. Some can be controlled in the GUI.
  struct settings_t
  {
    //! Notice the initialization of the class members in the constructor
    //! How is this happening?
    //!A default constructor,sets default values..
    /*!
     * It sets some parameters to some default values..<br>
     * view_center(0.0f, 20.0f) -> sets the center of view at(0,20)<br>
     * hz(60.0f) -> sets simulation step per frame to 60<br>
     * velocity_iterations(8) -> sets no. of velocity iterations to 8<br>
     * position_iterations(3) -> sets no. of position iterations to 3<br>
     * draw_shapes(1) -> sets draw status to 1 means all the shapes are shown in GUI<br>
     * draw_joints(1) -> sets joint status to 1 means all the joints are shown in GUI<br>
     * draw_AABBs(0) -> sets AABB status to 0 means all AABBs are not shown in GUI<br>
     * draw_pairs(0) -> sets pair status to 0 means pairs are not shown in GUI <br>
     * draw_contact_points(1) -> sets contact point status to 1 means all the contacts are shown in GUI<br>
     * draw_contact_normals(1) -> sets contact normals status to 1 means contact normals are shown in GUI<br>
     * draw_contact_forces(0) -> sets contact forces status to 0 means contact forces are not visible in GUI<br>
     * draw_friction_forces(0) -> sets friction forces status to 0 means friction forces are not visible in GUI<br>
     * draw_COMs(0) -> setscenter of mass status to 0 means center of mass of objects are not visible in GUI<br>
     * draw_stats(0) -> statistics status is 0 means statistics are not shown in GUI<br>
     * draw_profile(0) -> profiles status is 0 means profiles of bodies are not shown<br>
     * enable_warm_starting(1) -> warm starting status is 1 means warm starting takes place<br>
     * enable_continuous(1) -> continuity status is 1 means continuity takes place<br>
     * enable_sub_stepping(0) -> sub stepping  status is 0 means sub stepping doesn't takes place <br>
     * pause(0) -> pause is 0 means no pause<br>
     * single_step(0) -> single stepping status is set 0 means simulation plays continuously<br>
     */ 
    
    settings_t() :
      view_center(0.0f, 20.0f),
      hz(60.0f),
      velocity_iterations(8),
      position_iterations(3),
      draw_shapes(1),
      draw_joints(1),
      draw_AABBs(0),
      draw_pairs(0),
      draw_contact_points(1),
      draw_contact_normals(1),
      draw_contact_forces(0),
      draw_friction_forces(0),
      draw_COMs(0),
      draw_stats(0),
      draw_profile(0),
      enable_warm_starting(1),
      enable_continuous(1),
      enable_sub_stepping(0),
      pause(0),
      single_step(0)
    {}
    //!Center of the world
    b2Vec2 view_center;
    //!frame per second
    float32 hz;
    //!
    int32 velocity_iterations;
    //!
    int32 position_iterations;
    //!kinda boolean 1 if to show shapes and 0 for not showing shapes in GUI
    int32 draw_shapes;
    //!kinda boolean 1 if to show body joints and 0 for not showing shapes in GUI
    int32 draw_joints;
    //!kinda boolean 1 if to show AABB and 0 for not showing shapes in GUI
    int32 draw_AABBs;
    //!kinda boolean 1 if to show pairs and 0 for not showing shapes in GUI
    int32 draw_pairs;
    //!kinda boolean 1 if to show contact shapes and 0 for not showing shapes in GUI
    int32 draw_contact_points;
    //!kinda boolean 1 if to show contact normals and 0 for not showing shapes in GUI
    int32 draw_contact_normals;
    //!kinda boolean 1 if to show contact forces and 0 for not showing shapes in GUI
    int32 draw_contact_forces;
    //!kinda boolean 1 if to show friction forces and 0 for not showing shapes in GUI
    int32 draw_friction_forces;
    //!kinda boolean 1 if to show Center of Mass and 0 for not showing shapes in GUI
    int32 draw_COMs;
    //!kinda boolean 1 if to show statistics and 0 for not showing shapes in GUI
    int32 draw_stats;
    //!kinda boolean 1 if to show profiles and 0 for not showing shapes in GUI
    int32 draw_profile;
    //!kinda boolean 1 if warm starting is enabled otherwise 0
    int32 enable_warm_starting;
    //!kinda boolean 1 if continuity is enabled otherwise 0
    int32 enable_continuous;
    //!kinda boolean 1 if warm starting is enabled otherwise 0
    int32 enable_sub_stepping;
    //!kinda boolean 1 if paused otherwise 0
    int32 pause;
    //!kinda boolean 1 if single step feature is enabled otherwise 0
    int32 single_step;
  };
  //!struct that holds the name of the simulation and simulation itself under sim_t
  struct sim_t
  {
	 //!name takes the name of the simulation
    const char *name;
    sim_create_fcn *create_fcn;
	//!this one create object of the simulation by calling constuctor
    sim_t(const char *_name, sim_create_fcn *_create_fcn): 
      name(_name), create_fcn(_create_fcn) {;}
  };
  
  extern sim_t *sim;
  
  
  const int32 k_max_contact_points = 2048;  
  //!struct to define the contact point between two bodies
  struct contact_point_t
  {
	//!Fixture of body A in contact
    b2Fixture* fixtureA;
    //!Fixture of body B in contact
    b2Fixture* fixtureB;
    //!normal force etween the bodies of contact
    b2Vec2 normal;
    //!position of the contact happpen
    b2Vec2 position;
    //!state of the point of contact
    b2PointState state;
  };
  
  
  //!Class derived from b2ContactListenerv to gather contact details
  /*!
   * <pre>
   * This class is inherited from pre-defined class b2ContactListener
   * So as to implement contact listeners in the project. Many listeners are overrided to as to
   * use them in the most comfortable way.
   * It includes many listener fuctions like set_text_line(),draw_title(),step()
   * keyboard(),keyboard_up(),shift_mouse_down() etc.. to use them in our convinience way.
   * </pre>
   */
   
  class base_sim_t : public b2ContactListener
  {
  public:
    //! Constructor to initialize the parameters for simulation
    base_sim_t();

    //! Virtual destructors - amazing objects. to destroy all objects after simulation is over
    virtual ~base_sim_t();
    //!max. line is set when in need to display the stats. or profile
    void set_text_line(int32 line) { m_text_line = line; }
    //!set title as string at the point(x,y)
    void draw_title(int x, int y, const char *string);
    //!step function which takes input as settings(type setting_t*) and returns virtual
    virtual void step(settings_t* settings);
	//!sends signal that the key is used has no use and hence gives no error
    virtual void keyboard(unsigned char key) { B2_NOT_USED(key); }
    //!sends signal that the key is used has no use and hence gives no error
    virtual void keyboard_up(unsigned char key) { B2_NOT_USED(key); }
	//!sends signal that the mouse position got has no use and hence gives no error
    void shift_mouse_down(const b2Vec2& p) { B2_NOT_USED(p); }
    //!sends signal that the mouse position got has no use and hence gives no error
    virtual void mouse_down(const b2Vec2& p) { B2_NOT_USED(p); }
    //!sends signal that the mouse position got has no use and hence gives no error
    virtual void mouse_up(const b2Vec2& p) { B2_NOT_USED(p); }
    //!sends signal that the mouse position got has no use and hence gives no error
    void mouse_move(const b2Vec2& p) { B2_NOT_USED(p); }

    
    //! Let derived tests know that a joint was destroyed.
    //! if no joint is recieved to destroy then no error i given
    virtual void joint_destroyed(b2Joint* joint) { B2_NOT_USED(joint); }
    
    // Callbacks for derived classes.
    //!error is not given in case of no starting of contact
    virtual void begin_contact(b2Contact* contact) { B2_NOT_USED(contact); }
    //!error is not given in case of no ending of contact
    virtual void end_contact(b2Contact* contact) { B2_NOT_USED(contact); }
    
    virtual void pre_solve(b2Contact* contact, const b2Manifold* oldManifold);
    //!error is not given in case of no contact or no impluse
    virtual void post_solve(const b2Contact* contact, const b2ContactImpulse* impulse)
    {
      B2_NOT_USED(contact);
      B2_NOT_USED(impulse);
    }

    b2World* get_world(void)
    {
      return m_world;
    }
  //How are protected members different from private memebers of a class in C++ ?
  //!protected members
  protected:

    // What are Friend classes?
    //!contact_listener_t is a friend of this class which has permission to access protected and public members of the file
    friend class contact_listener_t; 
    
    //! variable to store ground body of type b2Body*
    b2Body* m_ground_body;
    //!variable to store AABB
    b2AABB m_world_AABB;
    //!array to store "k_max_contact_points" no. of maximum contact points 
    contact_point_t m_points[k_max_contact_points];
    //!counter to store the no. of contact points
    int32 m_point_count;

    debug_draw_t m_debug_draw;
    //!counter to store the no. of text lines
    int32 m_text_line;
    //! variable to store the world.
    b2World* m_world;
	//!variable to store the step count
    int32 m_step_count;
    //!max. profile
    b2Profile m_max_profile;
    //!total profile
    b2Profile m_total_profile;
  };
}

#endif
