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


#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296
{

	/**The dominos_t() Constructor.
   * The whole simulation runs here.
   * All objects have been placed and simulated in this constructor.
   */
  dominos_t::dominos_t()
  {
    //Ground  
    //!<B style="color:red">1. Ground</B>
    /*!
     * <pre>
     * defined by variable b1 of type b2Body
     * shape - variable of type b2EdgeShape for shape of ground
     * shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f)) - set shape of gound as edge
     * bd - variable of type b2BodyDef which holds the data for the rigid body ground
     * m_world->CreateBody(&bd) - create ground body in world m_world
     * b1->CreateFixture(&shape, 0.0f) - Creates a fixture from shape of density 5 and attach it to its body.
     * </pre>
     */
    b2Body* b1;
    {
      b2EdgeShape shape;
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
	
      b2BodyDef bd;
      b1 = m_world->CreateBody(&bd);
      b1->CreateFixture(&shape, 0.0f);
    }


    //Motored Plank  
    //!<B style="color:red">2. Motored Plank</B>
    /*!
     * <pre>
     * This is to push the ball so that ball colides to the dominos.
     * It is defined by variable body of type b2Body(dynamic)
     * shape - variable of type b2EdgeShape for shape of plank
     * position is set to (-47.0,42.0)
     * shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f)) - set shape of gound as edge
     * bd - variable of type b2BodyDef which holds the data for the rigid body plank
     * m_world->CreateBody(&bd) - create ground body in world m_world
     * body->CreateFixture(&shape, 0.0f) - Creates a fixture from shape of density 30 and attach it to its body.
     * body2 is declared without fixtures to join the plank with b2RevoluteJoint
     * This joint is motored with Torque=100 units and motorspeed=10 units
     * </pre>
     */
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.2f, 2.2f);
     
      b2BodyDef bd;
      bd.position.Set(-47.0f, 42.0f);
      bd.type = b2_dynamicBody;
      b2Body* body = m_world->CreateBody(&bd);
      b2FixtureDef *fd = new b2FixtureDef;
      fd->density = 30.0f;
      fd->shape = new b2PolygonShape;
      fd->shape = &shape;
      body->CreateFixture(fd);
 
      b2PolygonShape shape2;
      shape2.SetAsBox(2.0f, 0.2f);
      b2BodyDef bd2;
      bd2.position.Set(-47.0f, 43.0f);
      b2Body* body2 = m_world->CreateBody(&bd2);
 
      b2RevoluteJointDef jointDef;
      jointDef.bodyA = body;
      jointDef.bodyB = body2;
      jointDef.localAnchorA.Set(0,0);
      jointDef.localAnchorB.Set(0,0);
      jointDef.maxMotorTorque = 100.0f;
      jointDef.motorSpeed = 10.0f;
      jointDef.enableMotor = true;
      jointDef.collideConnected = false;
      m_world->CreateJoint(&jointDef);
    }

    
    //top slope
    //!<B style="color:red">3. Top Slope</B>
    /*!
    	* <pre>
    	* Ball Rolls down from this slope to strike the dominos.
    	* made from small-small 10 fixed edges to seem like curve.
    	* It Starts from (-36,40)
    	* </pre>
    	*/	
    { 
      b2Body* tslop;
      float32 x = -36.0f;
      float32 y = 40.0f;
      for(int i = 0 ; i < 10 ; i++){
      	b2EdgeShape curve2;
	      b2BodyDef curve2bd;
   	     curve2.Set(b2Vec2(x,y) , b2Vec2(x+(i+2)*0.1 , y-(i)*0.1));
   	     x = x+(i+2)*0.1;
   	     y = y-(i)*0.1;
   	     tslop = m_world->CreateBody(&curve2bd);
      	  tslop->CreateFixture(&curve2,0.0f);
      }
    }
    //Ball which rolls down on slope
    //!<B style="color:red">4. Ball which rolls down on slope</B>
    /*!
     * <pre>
     * This is the Ball which rolls down and hits the dominos
     * body of ball is defined as name 'sbody' of shape circle : type b2CircleShape
     * ball has density -> 1.4 , friction -> 0
     * </pre>
     */
    {
      b2Body* sbody;
      b2CircleShape circle;
      circle.m_radius = 2.0;
  
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 1.40f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(-43.80f, 40.50f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }

    //Platform on which the above ball is initially kept
    //!<B style="color:red">5. Platform on which the above ball is initially kept</B>
    /*!
     * <pre>
     * Platform is of Dimension 5x0.25 at (-41,40)
     * body of ball is defined as name 'plank1' of shape 'shape' : type b2PolygonShape
     * ball has density -> 1.4 , friction -> 0 , 
     * </pre>
     */
    {
      b2PolygonShape shape;
      shape.SetAsBox(5.0f, 0.25f);
  
      b2BodyDef bd;
      bd.position.Set(-41.0f, 40.0f);
      b2Body* plank1 = m_world->CreateBody(&bd);
      plank1->CreateFixture(&shape, 0.0f);
    }

      
    //Top horizontal shelf on which train of dominos are kept
    //!<B style="color:red">6. Top horizontal shelf on which train of dominos are kept</B>
    /*!
     * <pre>
     * Shelf is of Dimension 10x0.25 at position (-20,35)
     * body of shelf is defined as name 'shelf1' of shape 'shape' : type b2PolygonShape
     * shelf has density -> 0, 
     * </pre>
     */
    {
      b2PolygonShape shape;
      shape.SetAsBox(10.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-20.0f, 35.0f);
      b2Body* shelf1 = m_world->CreateBody(&bd);
      shelf1->CreateFixture(&shape, 0.0f);
    }

    //Train of Dominos
    //!<B style="color:red">7. Train of Dominos</B>
    /*!
     * <pre>
     * This train falls when ball strikes them and then they strike the revolving platform which strikes to another train of Dominos.
     * Train is of size 14 and each Domino is of dimension (0.1,1.7) has density 20 units.
     * Located at height 36.25 and are separated by 1 unit starting from -24.5.
     * body of domino is defined as name 'body' of shape 'shape' : type b2PolygonShape
     * </pre>
     */
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.1f, 1.70f);
	
      b2FixtureDef fd;
      fd.shape = &shape;
      fd.density = 20.0f;
      fd.friction = 0.1f;
		
      for (int i = 0; i < 14; ++i)
		{
		  b2BodyDef bd;
		  bd.type = b2_dynamicBody;
		  bd.position.Set(-24.5f + 1.0f * i, 36.25f);
		  b2Body* body = m_world->CreateBody(&bd);
		  body->CreateFixture(&fd);
		}
    }
    
	 //The revolving platform
    //!<B style="color:red">8. The revolving platform</B>
    /*!
     * <pre>
     * This is get struk by dominos and it strike other train of dominos
     * It is defined by variable body of type b2Body(dynamic)
     * shape - variable of type b2EdgeShape for shape of platform
     * position is set to (-8.30f, 34.0f)
     * shape.Set(0.3f, 5.8f) - set shape of platform as edge
     * bd - variable of type b2BodyDef which holds the data for the rigid body platform
     * m_world->CreateBody(&bd) - create ground body in world m_world
     * body->CreateFixture(&shape) - Creates a fixture and attach it to its body.
     * body2 is declared without fixtures to join the platform with help of b2RevoluteJointDef : jointDef
     * This joint is simple.
     * </pre>
     */ 
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.3f, 5.8f);
  
      b2BodyDef bd;
      bd.position.Set(-8.30f, 34.0f);
      bd.type = b2_dynamicBody;
      b2Body* body = m_world->CreateBody(&bd);
      b2FixtureDef *fd = new b2FixtureDef;
      fd->density = 1.f;
      fd->shape = new b2PolygonShape;
      fd->shape = &shape;
      body->CreateFixture(fd);

      b2PolygonShape shape2;
      shape2.SetAsBox(0.3f, 5.80f);
      b2BodyDef bd2;
      bd2.position.Set(-8.30f, 34.0f);
      b2Body* body2 = m_world->CreateBody(&bd2);

      b2RevoluteJointDef jointDef;
      jointDef.bodyA = body;
      jointDef.bodyB = body2;
      jointDef.localAnchorA.Set(0,0);
      jointDef.localAnchorB.Set(0,0);
      jointDef.collideConnected = false;
      m_world->CreateJoint(&jointDef);
    
    }

	 //Top Second horizontal shelf on which another train of dominos are kept
    //!<B style="color:red">9. Top Second horizontal shelf on which another train of dominos are kept</B>
    /*!
     * <pre>
     * This holds the train of dominos
     * Shelf is of Dimension 17x0.25 at position (-24,27)
     * body of shelf is defined as name 'shelf2' of shape 'shape' : type b2PolygonShape
     * shelf has density -> 0, 
     * </pre>
     */

    b2Body* shelf2;
    {
      b2PolygonShape shape;
      shape.SetAsBox(17.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-24.0f, 27.0f);
      shelf2 = m_world->CreateBody(&bd);
      shelf2->CreateFixture(&shape, 0.0f);
    }

	 //Another Train of Dominos
    //!<B style="color:red">10. Another Train of Dominos</B>
    /*!
     * <pre>
     * This train falls when revolving platform strikes them and then they strike the half dumble which also strikes another revolving platform.
     * Train is of size 28 and each Domino is of dimension (0.1,1.7) has density 20 units.
     * Located at height 28.25 and are separated by 1 unit starting from -39.65.
     * body of domino is defined as name 'dominos2' of shape 'shape2' : type b2PolygonShape
     * </pre>
     */    
   b2Body* dominos2;
   {
      b2PolygonShape shape2;
      shape2.SetAsBox(0.1f, 1.70f);
  
      b2FixtureDef fd2;
      fd2.shape = &shape2;
      fd2.density = 20.0f;
      fd2.friction = 0.1f;
    
      for (int i = 1; i < 29; ++i)
      {
       	b2BodyDef bd2;
         bd2.type = b2_dynamicBody;
         bd2.position.Set(-39.65f + 1.0f * i, 28.25f);
         dominos2 = m_world->CreateBody(&bd2);
         dominos2->CreateFixture(&fd2);
      }
   }




    //Half Dumble at the end of train of dominos2 which on revolving hits the Newton's Cradle
    //!<B style="color:red">11. Half Dumble at the end of train of dominos2 which on revolving hits the Newton's Cradle</B>
    /*!
     * <pre>
     * Made up of two bodies. A thin box and a ball attached to end of it.
     * This is get struk by dominos and it strike the revolving platform
     * First part is defined by variable 'sbodydom' of type b2Body(dynamic)
     * Second part is defined by variable 'sbody' of type b2Body(dynamic)
     * These two parts are then joint with anchor located at position (-39.72f, 30.50f)
     * shape2 - variable of type b2EdgeShape for shape of box
     * circle - variable of type b2CircleShape for shape of ball
     * position is set to (-39.65f , 28.23f) and (-39.72f, 30.50f) respectively.
     * bd2 - variable of type b2BodyDef which holds the data for the rigid body box
     * ballbd - variable of type b2BodyDef which holds the data for the rigid body ball
     * m_world->CreateBody(&bd2) - create box body in world m_world
     * m_world->CreateBody(&ballbd) - create ball body in world m_world
     * body->CreateFixture(&fd2) - Creates a fixture and attach it to box body.
     * jd - joint type b2RevoluteJointDef.
     * jd.Initialize(sbody, sbodydom, anchor) - initializes the joint
     * m_world->CreateJoint(&jd) - creates the joint
     * </pre>
     */
   {
   
   //Thin Box of Dumble
   b2Body* sbodydom; 
   	{    
  			b2PolygonShape shape2;
    		shape2.SetAsBox(0.2f, 2.50f);
  
         b2FixtureDef fd2;
    	   fd2.shape = &shape2;
    	   fd2.density = 18.0f;
    	   fd2.friction = 0.1f;
    
    		b2BodyDef bd2;
    		bd2.type = b2_dynamicBody;
    		bd2.position.Set(-39.65f , 28.23f);
    		sbodydom = m_world->CreateBody(&bd2);
    		sbodydom->CreateFixture(&fd2);
   	}

   // Sphere of Dumble
   b2Body* sbody;
   	{
      	b2CircleShape circle;
      	circle.m_radius = 1.0;
  
      	b2FixtureDef ballfd;
      	ballfd.shape = &circle;
      	ballfd.density = 10.0f;
      	ballfd.friction = 0.0f;
      	ballfd.restitution = 0.0f;
      	b2BodyDef ballbd;
      	ballbd.type = b2_dynamicBody;
      	ballbd.position.Set(-39.72f, 30.50f);
      	sbody = m_world->CreateBody(&ballbd);
      	sbody->CreateFixture(&ballfd);
   	}
    
    //Joint
   	{
    		b2RevoluteJointDef jd;
      	b2Vec2 anchor;
      	anchor.Set(-39.72f, 30.50f);
      	jd.Initialize(sbody, sbodydom, anchor);
    		m_world->CreateJoint(&jd);    
		}
	}
	
	
	//The Left Revolvingr Platform
	//!<B style="color:red">12. The Left Revolving Platform</B>
    /*!
     * <pre>
     * This is get struk by dominos and it strikes Newtons Cradle.
     * It is defined by variable 'sbodydom1' of type b2Body(dynamic)
     * shape - variable of type b2EdgeShape for shape of platform
     * position is set to (-43.30f, 27.0f)
     * shape.Set(0.3f, 5.8f) - set shape of platform as edge
     * bd - variable of type b2BodyDef which holds the data for the rigid body platform
     * m_world->CreateBody(&bd) - create ground body in world m_world
     * body->CreateFixture(&shape) - Creates a fixture and attach it to its body.
     * sbodydom2 is declared without fixtures to join the platform with help of b2RevoluteJointDef : jointDef
     * jointDef.collideConnected = false - ensures that collision of these two bodies is not accounted.
     * This joint is simple (not motor joint).
     * </pre>
     */
	{	
      b2PolygonShape shape;
      shape.SetAsBox(0.3f, 5.8f);
	
		
		b2BodyDef bd;
		bd.position.Set(-43.0f , 27.0f);
		bd.type = b2_dynamicBody;
		b2Body* sbodydom1 = m_world->CreateBody(&bd);
		b2FixtureDef *fd = new b2FixtureDef;
		fd->density = 2.f;
		fd->shape = new b2PolygonShape;
		fd->shape = &shape;
		sbodydom1->CreateFixture(fd);
    
    
    
		b2PolygonShape shape2;
		shape2.SetAsBox(0.3f, 5.80f);
		b2BodyDef bd2;
		bd2.position.Set(-43.0f , 27.0f);
		b2Body* sbodydom2 = m_world->CreateBody(&bd2);
	
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = sbodydom1;
		jointDef.bodyB = sbodydom2;
		jointDef.localAnchorA.Set(0,0);
		jointDef.localAnchorB.Set(0,0);
		jointDef.collideConnected = false;
		m_world->CreateJoint(&jointDef);
	    
     }


	//newton's cradle
	//!<B style="color:red">13. Newton's Cradle</B> 
	/*!
	 * <pre>
	 * It is a series of 7 swinging identically sized and shaped spheres so that they are just touching each other at rest. It is placed just below the second horizontal shelf. Revolving platform hits it and the last sphere hits another sphere resting on the third horizontal shelf.
	 * Each is defined by variable 'sbodydom1' of type b2Body(dynamic)
    * cshape - variable of type b2CircleShape for shape of sphere, radius 1.
    * Their center are separated by the distance of diameter and are located starting from (-48.30f,21.0f).
    * bd1 - variable of type b2BodyDef which holds the data for the sphere
    * m_world->CreateBody(&bd1) - create sphere body in world m_world
    * body->CreateFixture(&shape) - Creates a fixture and attach it to its body.
    * Each sphere is hung from a point body of size(0.1x0.1)
    * point - variable of type b2BodyDef which holds the data for the point.
    * pshape - shape of type b2PolygonShape.
    * m_world->CreateBody(&pointbd) - creates point body
    * rp - variable type b2RopeJointDef
    * bodies and anchors are assigned to joint defination
    * rp.maxLength=((point->GetPosition()) - sbodydom1->GetPosition() ).Length() - measures the max length of rope as current distance between bodies
	 * </pre>
	 */
 	{  
      b2CircleShape cshape;
      cshape.m_radius=1.0f;
      
  
    for (int i = 0; i < 7; ++i)
    {
      b2BodyDef bd1;
      bd1.position.Set(-40.30f + 2.0f * i, 21.0f);
      bd1.type = b2_dynamicBody;
      b2Body* sbodydom1 = m_world->CreateBody(&bd1);
      b2FixtureDef *fd = new b2FixtureDef;
      fd->density = 1.0f;
      fd->shape = new b2CircleShape;
      fd->shape = &cshape;
      sbodydom1->CreateFixture(fd);
    
      b2Body* point;
      b2PolygonShape pshape;
      pshape.SetAsBox(0.1f,0.1f); 
      b2BodyDef pointbd;
      pointbd.position.Set(-40.30f + 2.0f * i, 26.5f);
      point = m_world->CreateBody(&pointbd);
      point->CreateFixture(&pshape, 0.0f);

      
      b2RopeJointDef rp;
      rp.bodyA=point; 
      rp.bodyB=sbodydom1;
      rp.localAnchorA = b2Vec2(0,0);
      rp.localAnchorB = b2Vec2(0,0);
      rp.maxLength=((point->GetPosition()) - sbodydom1->GetPosition() ).Length();
      m_world->CreateJoint(&rp);
     }
  }




//3 slab for initiation of boll && boll in pulley
//14
{
	{
      b2PolygonShape shape;
      shape.SetAsBox(10.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-16.0f, 20.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }
    
        b2Body* sbody2;
   {
      b2CircleShape circle;
      circle.m_radius = 1.50;
	
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 1.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(-25.0f, 20.30f);
      sbody2 = m_world->CreateBody(&ballbd);
      sbody2->CreateFixture(&ballfd);
    }

    //The pulley system
    {
      b2BodyDef *bd = new b2BodyDef;
      bd->type = b2_dynamicBody;
      bd->position.Set(-2,17);
      bd->fixedRotation = true;
      
      //The open box
      b2FixtureDef *fd1 = new b2FixtureDef;
      fd1->density = 10.0;
      fd1->friction = 0.5;
      fd1->restitution = 0.f;
      fd1->shape = new b2PolygonShape;
      b2PolygonShape bs1;
      bs1.SetAsBox(2,0.2, b2Vec2(0.f,-1.9f), 0);
      fd1->shape = &bs1;
      b2FixtureDef *fd2 = new b2FixtureDef;
      fd2->density = 10.0;
      fd2->friction = 0.5;
      fd2->restitution = 0.f;
      fd2->shape = new b2PolygonShape;
      b2PolygonShape bs2;
      bs2.SetAsBox(0.2,2, b2Vec2(2.0f,0.f), 0);
      fd2->shape = &bs2;
      b2FixtureDef *fd3 = new b2FixtureDef;
      fd3->density = 10.0;
      fd3->friction = 0.5;
      fd3->restitution = 0.f;
      fd3->shape = new b2PolygonShape;
      b2PolygonShape bs3;
      bs3.SetAsBox(0.2,2, b2Vec2(-2.0f,0.f), 0);
      fd3->shape = &bs3;
       
      b2Body* box1 = m_world->CreateBody(bd);
      box1->CreateFixture(fd1);
      box1->CreateFixture(fd2);
      box1->CreateFixture(fd3);

      //The bar
      bd->position.Set(7,17);	
      fd1->density = 30.0;	  
      b2Body* box2 = m_world->CreateBody(bd);
      box2->CreateFixture(fd1);

      // The pulley joint
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();
      b2Vec2 worldAnchorOnBody1(-2, 18); // Anchor point on body 1 in world axis
      b2Vec2 worldAnchorOnBody2(7, 18); // Anchor point on body 2 in world axis
      b2Vec2 worldAnchorGround1(-2, 23); // Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(7, 23); // Anchor point for ground 2 in world axis
      float32 ratio = 1.0f; // Define ratio
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);
    }

}

    //Revolving Platform
    //!<B style="color:red">15. Revolving Platform</B>
	  /*!
	   * <pre>
     * This platform has spherical ball resting on it whose motion is triggered when rhs of pulley hits the plank.
     * The ball falls and follows the zig-zag path.
	   *  body - type b2Body* variable to declare rhp - revolving horizontal platform
	   * body2 - type b2Body* variable to declare dummy body to join body with joint
	   * fd - declaring fixture variable of type b2FixtureDef for rhp
	   * jointDef.collideConnected = false - so that body and body2 do not collide
     * </pre>
     */
    {
      b2PolygonShape shape;			//declaring variable for rhp body
      shape.SetAsBox(2.2f, 0.2f);	//setting shape of the rhp body
	
      b2BodyDef bd;					//declaring variable for defining rhp body
      bd.position.Set(10.0f, 20.0f);//setting postion of rhp body
      bd.type = b2_dynamicBody;		//setting rhp body as dynamic type
      b2Body* body = m_world->CreateBody(&bd);	//creating rhp body
      b2FixtureDef *fd = new b2FixtureDef;		//declaring fixture variable for rhp
      fd->density = 1.0f;						//setting density
      fd->shape = new b2PolygonShape;			//declaring polygon shape 
      fd->shape = &shape;						//assigning rhp body shape 
      body->CreateFixture(fd);					//creating fixture for rhp body

  	//The Revolving horizontal bar on which larger sphereical ball is kept
      b2PolygonShape shape2;					//variable for the shape of the bar
      shape2.SetAsBox(8.2f, 2.0f);				//setting shape of the bar
      b2BodyDef bd2;							//variable to define the bar
      bd2.position.Set(10.0f, 21.0f);			//setting the postion of bar
      b2Body* body2 = m_world->CreateBody(&bd2);//declaring and creating the body of bar

  	//creaing revolving joint to revolving bar
      b2RevoluteJointDef jointDef;				//declaring joint
      jointDef.bodyA = body;              //body A of joint
      jointDef.bodyB = body2;             //body B of joint
      jointDef.localAnchorA.Set(0,0);     //setting anchor A to (0,0)
      jointDef.localAnchorB.Set(0,0);     //setting anchor B to (0,0)
      jointDef.collideConnected = false;  //attached bodies must not collide
      m_world->CreateJoint(&jointDef);    //creating joint
    }
///////////////////////////////////////////

  //The heavy sphere on the platform
  //!<B style="color:red">16. The heavy sphere on the platform</B>
  /*!
   * <pre>
   * This is the Ball which rolls down and hits the dominos
   * body of ball is defined as name 'sbody' of shape circle : type b2CircleShape :radius = 1
   * ball has density -> 50 , friction -> 0
   * ballbd - type b2BodyDef holds the defination of ball and is dynamic
   * ballbd.position.Set(10.0f, 23.0f) - sets the position to (10,23)
   * m_world->CreateBody(&ballbd) - sbody is created
   * CreateFixture(&ballfd) - fixtures are asigned
   * </pre>
   */
   {
      b2Body* sbody;
      b2CircleShape circle;
      circle.m_radius = 1.0;
  
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 50.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(10.0f, 23.0f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }

    //Side Slant Curve1
    //!<B style="color:red">17. Side Slant Curve1</B>
    /*!
     * <pre>
     * Ball Rolls down from this slope to push the piston box.
     * made from small-small 10 fixed edges to seem like curve.
     * It Starts from (11.2,19)
     * m_world->CreateBody(&curve1bd) - create line
     * CreateFixture(&curve1,0.0f) - sets fixtures.
     * </pre>
     */
    {
      b2Body* basecurve1;
      float32 x = 11.2f;
      float32 y = 19.0f;
      for(int i = 0 ; i < 10 ; i++){
      	b2EdgeShape curve1;
      	b2BodyDef curve1bd;
        	curve1.Set(b2Vec2(x,y) , b2Vec2(x+(i+2)*0.1 , y-(9-i)*0.1));
        	x = x+(i+2)*0.1 ;
        	y = y-(9-i)*0.1;
        	basecurve1 = m_world->CreateBody(&curve1bd);
        	basecurve1->CreateFixture(&curve1,0.0f);
      }
    } 


    //Side Slant Curve2
    //!<B style="color:red">18. Side Slant Curve2</B>
    /*!
     * <pre>
     * Similar but miiror image of previous curve
     * </pre>
     */
    {
      b2Body* basecurve2;
      float32 x = 25.0f;
      float32 y = 15.0f;
      for(int i = 0 ; i < 10 ; i++){
      	b2EdgeShape curve2;
      	b2BodyDef curve2bd;
     	   curve2.Set(b2Vec2(x,y) , b2Vec2(x-(i+2)*0.1 , y-(9-i)*0.1));
     	   x = x-(i+2)*0.1;
     	   y = y-(9-i)*0.1;
    	   basecurve2 = m_world->CreateBody(&curve2bd);
      	basecurve2->CreateFixture(&curve2,0.0f);
      }
    } 


    //Lower Slab
    //!<B style="color:red">19. Lower Slab</B>
    /*!
     * <pre>
     * This Slab is kind of piston tigger, when hit by sphere it pushes another slab attached from balloon in piston which comes out of piston.
     * It is defined by variable body of type b2Body*
     * slab - variable of type b2EdgeShape for shape of platform
     * position is set to (-20.50f, 02.0f)
     * slab.Set(2.1f, 2.8f) - set shape of platform as edge
     * bd - variable of type b2BodyDef which holds the data for the rigid body platform
     * m_world->CreateBody(&bd) - create ground body in world m_world.
     * fd - type b2FixtureDef holds the fixture.
     * body->CreateFixture(&fd) - Creates a fixture and attach it to its body.
     * </pre>
     */
    {
      b2PolygonShape slab;
      slab.SetAsBox(2.1f, 2.80f);
  
      b2FixtureDef fd;
      fd.shape = &slab;
      fd.density = 1.0f;
      fd.friction = 0.1f;
    
      b2BodyDef bd;
      bd.type = b2_dynamicBody;
      bd.position.Set(-20.5f , 02.0f);
      b2Body* body = m_world->CreateBody(&bd);
      body->CreateFixture(&fd);
    }

    //Piston Body
    //!<B style="color:red">20. Piston Body</B>
    /*!
     * <pre>                                           _   
     * This is made up of three bodies of the shape '_| ' (reverse Z).
     * bd, up1bd, upv1bd - variables of type b2BodyDef which holds the data for the rigid platform.
     * (b2BodyDef).position.Set(x,y) - sets the position of body.
     * m_world->CreateBody(&bd) - create ground body in world m_world.
     * fd - type b2FixtureDef holds the fixture.
     * body->CreateFixture(&fd) - Creates a fixture and attach it to its body.
     * </pre>
     */
    {
      b2PolygonShape shape;
      shape.SetAsBox(2.0f, 0.25f);
  
      b2BodyDef bd;
      bd.position.Set(-30.0f, 3.7f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);

      //uper1
      b2PolygonShape up1shape;
      up1shape.SetAsBox(5.0f, 0.25f);
  
      b2BodyDef up1bd;
      up1bd.position.Set(-23.0f, 6.0f);
      b2Body* up1ground = m_world->CreateBody(&up1bd);
      up1ground->CreateFixture(&up1shape, 0.0f);
     
      //upv1
      b2PolygonShape upv1shape;
      upv1shape.SetAsBox(0.25f, 1.0f);
  
      b2BodyDef upv1bd;
      upv1bd.position.Set(-28.0f, 5.0f);
      b2Body* upv1ground = m_world->CreateBody(&upv1bd);
      upv1ground->CreateFixture(&upv1shape, 0.0f);
    }


    //Balloon Connected with slab 
    //!<B style="color:red">21. Balloon Connected with slab</B>
    /*!
     * <pre>   
     * This is made up of two bodies. One is slab and other is balloon.
     * <B style="color:yellow">For Balloon</B>
     * circle - variable type b2CircleShape and radius 2.
     * ballbd - variables of type dyanmic b2BodyDef which holds the data for the ball. Density,fricrion are assigned 1.4,0 respectively.
     * (b2BodyDef).position.Set(x,y) - sets the position of body.
     * m_world->CreateBody(&ballbd) - create ball body in world m_world.
     * fd - type b2FixtureDef holds the fixture.
     * sbody - type b2Body*
     * sbody->CreateFixture(&ballfd) - Creates a fixture and attach it to its body.
     * sbody->SetGravityScale(-1) - sets reverse gravity for the balloon to make it rise.
     * <B style="color:yellow">For Slab</B>
     * slab - shape type b2PolygonShape box size (6.1X1.7)
     * bd - type b2BodyDef holds defination of body slabbody
     * density 0.5 and friction 0.0f is assigned
     * bd.position.Set(-26.5f , 02.0f) - position is assigned to (-26.5f , 02.0f).
     * These two bodies are joint with rope joint rp type b2RopeJointDef.
     * rp.maxLength=((sbody->GetPosition()) - slabbody->GetPosition() ).Length() - define max length of joint = current distance between bodies
     * m_world->CreateJoint(&rp) - create joint rp in m_world
     * </pre>
     */
    {
    b2Body* sbody;
    {
    
      b2CircleShape circle;
      circle.m_radius = 2.0;
  
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 1.40f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(-34.80f, 8.50f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->SetGravityScale(-1);//cancel gravity (use -1 to reverse gravity, etc)
      sbody->CreateFixture(&ballfd);
    }

      //lower slab
      b2Body* slabbody;
    {
      b2PolygonShape slab;
      slab.SetAsBox(6.1f, 1.70f);
  
      b2FixtureDef fd;
      fd.shape = &slab;
      fd.density = .50f;
      fd.friction = 0.0f;
    
      b2BodyDef bd;
      bd.type = b2_dynamicBody;
      bd.position.Set(-26.5f , 02.0f);
      slabbody = m_world->CreateBody(&bd);
      slabbody->CreateFixture(&fd);
    }
    //rope joint
      b2RopeJointDef rp;
      rp.bodyA=sbody; //define bodies
      rp.bodyB=slabbody;
      rp.localAnchorA = b2Vec2(0,0); //define anchors
      rp.localAnchorB = b2Vec2(-6,0);
      rp.maxLength=((sbody->GetPosition()) - slabbody->GetPosition() ).Length(); //define max length of joint = current distance between bodies
      m_world->CreateJoint(&rp); //create joint    

    }

  }  
  
  sim_t *sim = new sim_t("Dominos", dominos_t::create);

}
