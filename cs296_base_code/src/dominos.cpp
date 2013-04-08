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
      b2EdgeShape curve2[10];
      b2BodyDef curve2bd[10];
      float32 x = -36.0f;
      float32 y = 40.0f;
      for(int i = 0 ; i < 10 ; i++){
        curve2[i].Set(b2Vec2(x,y) , b2Vec2(x+(i+2)*0.1 , y-(i)*0.1));
        x = x+(i+2)*0.1;
        y = y-(i)*0.1;
        tslop = m_world->CreateBody(&curve2bd[i]);
        tslop->CreateFixture(&curve2[i],0.0f);
      }
    }
    //Ball which rolls down on slope
    //!<B style="color:red">4. Ball which rolls down on slope</B>
    /*!
     * <pre>
     * This is the Ball which rolls down and hits the dominos
     * body of ball is defined as name 'sbody' of shape circle : type b2CircleShape
     * ball has density -> 1.4 , friction -> 0 , 
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
     * Platform is of Dimension 5x0.25
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

      
    //Top horizontal shelf
    {
      b2PolygonShape shape;
      shape.SetAsBox(10.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-20.0f, 35.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }

    //Dominos
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
    






    // sravi The revolving horizontal platform
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



//*********
  
    //ravi Top Second horizontal shelf
    b2Body* ground;
    {
      b2PolygonShape shape;
      shape.SetAsBox(17.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-24.0f, 27.0f);
      ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }

//on second shelves    
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




    //for rotating half dumble
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
  
   

  
    // sphere of dumble
   
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
   
   
    
    b2RevoluteJointDef jd;
      b2Vec2 anchor;
      anchor.Set(-39.72f, 30.50f);
      jd.Initialize(sbody, sbodydom, anchor);
    m_world->CreateJoint(&jd);    



//newton's cradle
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
      rp.bodyA=point; //define bodies
      rp.bodyB=sbodydom1;
      rp.localAnchorA = b2Vec2(0,0); //define anchors
      rp.localAnchorB = b2Vec2(0,0);
      rp.maxLength=((point->GetPosition()) - sbodydom1->GetPosition() ).Length(); //define max length of joint = current distance between bodies
      m_world->CreateJoint(&rp); //create joint
     }
  }


			
		
	 
	
		
	}  
//////////////////////////////////////////
//left rorater domble
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


//3 slab for initiation of boll && boll in pulley

	{
      b2PolygonShape shape;
      shape.SetAsBox(10.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-16.0f, 20.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }
    
        b2Body* sbody;
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
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
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






////////////////////////////////////////////////
    {
	/*!
	* <pre>
	* body - type b2Body* variable to declare rhp
	* body2 - type b2Body* variable to declare dummy body to join body with joint
	* fd - declaring fixture variable of type b2FixtureDef for rhp
	* jointDef.collideConnected = false - so that body and body2 do not collide
    * </pre>
    */
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
//ravi rotater 
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

    //side slant curve
    {
      b2Body* basecurve1;
      b2EdgeShape curve1[10];
      b2BodyDef curve1bd[10];
      float32 x = 11.2f;
      float32 y = 19.0f;
      for(int i = 0 ; i < 10 ; i++){
        curve1[i].Set(b2Vec2(x,y) , b2Vec2(x+(i+2)*0.1 , y-(9-i)*0.1));
        x = x+(i+2)*0.1 ;
        y = y-(9-i)*0.1;
        basecurve1 = m_world->CreateBody(&curve1bd[i]);
        basecurve1->CreateFixture(&curve1[i],0.0f);
      }
    } 


    //side slant platform2
    {
      b2Body* basecurve2;
      b2EdgeShape curve2[10];
      b2BodyDef curve2bd[10];
      float32 x = 25.0f;
      float32 y = 15.0f;
      for(int i = 0 ; i < 10 ; i++){
        curve2[i].Set(b2Vec2(x,y) , b2Vec2(x-(i+2)*0.1 , y-(9-i)*0.1));
        x = x-(i+2)*0.1;
        y = y-(9-i)*0.1;
        basecurve2 = m_world->CreateBody(&curve2bd[i]);
        basecurve2->CreateFixture(&curve2[i],0.0f);
      }
    } 


    //lower slab2
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

    //pisten 
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


        //bubble
    {
     
    b2Body* sbody;
    {
      // b2Vec2 gravity(0.0f, 0f);
      // bool doSleep = false;
      // b2World* g_World = new b2World(gravity, doSleep);

      
      //b2Body* sbody;
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
      
      //sbody->ApplyForce( sbody->GetMass() * -m_world->GetGravity(), sbody->GetWorldCenter() );
      //Box2D v2.2.1 onwards
      sbody->SetGravityScale(-1);//cancel gravity (use -1 to reverse gravity, etc)
      sbody->CreateFixture(&ballfd);

     // sbody->setLinearVelocity(0, -10);

      }

      //lower slab
      b2Body* slabbody;
    {
      b2PolygonShape slab;
      slab.SetAsBox(6.1f, 1.70f);
  
      b2FixtureDef fd;
      fd.shape = &slab;
      fd.density = .50f;
      fd.friction = 0.1f;
    
      b2BodyDef bd;
      bd.type = b2_dynamicBody;
      bd.position.Set(-26.5f , 02.0f);
      slabbody = m_world->CreateBody(&bd);
      slabbody->CreateFixture(&fd);
    }
    //roop
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
