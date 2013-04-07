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
  dominos_t::dominos_t()
  {
    //Ground (b1)  > lower most horigental line
    b2Body* b1;
    {
      b2EdgeShape shape;
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
	
      b2BodyDef bd;
      b1 = m_world->CreateBody(&bd);
      b1->CreateFixture(&shape, 0.0f);
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
    
    
    
    //ravi Top horizontal shelf
    b2Body* ground;
    {
      b2PolygonShape shape;
      shape.SetAsBox(14.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-21.0f, 27.0f);
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
		
      for (int i = 1; i < 23; ++i)
	{
	  b2BodyDef bd2;
	  bd2.type = b2_dynamicBody;
	  bd2.position.Set(-33.65f + 1.0f * i, 28.25f);
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
	  bd2.position.Set(-33.65f , 28.23f);
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
      ballbd.position.Set(-33.72f, 30.50f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }
	 
	 
	  
	  b2RevoluteJointDef jd;
      b2Vec2 anchor;
      anchor.Set(-33.72f, 30.50f);
      jd.Initialize(sbody, sbodydom, anchor);
	  m_world->CreateJoint(&jd);
  
      
   
		//joining of dominp with ground
	/*
	b2RevoluteJointDef jd1;
      b2Vec2 anchor1;
      anchor.Set(-33.65f, 28.0f);
      jd1.Initialize(sbodydom, ground, anchor1);
      m_world->CreateJoint(&jd1);
    
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
	
		
	
		
	}  
//////////////////////////////////////////
//left rorater domble
{	
      b2PolygonShape shape;
      shape.SetAsBox(0.3f, 5.8f);
	
		
		b2BodyDef bd;
		bd.position.Set(-37.30f , 27.0f);
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
		bd2.position.Set(-37.30f , 27.0f);
		b2Body* sbodydom2 = m_world->CreateBody(&bd2);
	
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = sbodydom1;
		jointDef.bodyB = sbodydom2;
		jointDef.localAnchorA.Set(0,0);
		jointDef.localAnchorB.Set(0,0);
		jointDef.collideConnected = false;
		m_world->CreateJoint(&jointDef);
	    
     }


///////////////////////////////////////////////
	//lower  hanging domble 
	
	{	
      b2PolygonShape shape;
      shape.SetAsBox(0.3f, 3.8f);
	
		for (int i = 0; i < 4; ++i)
		{
		b2BodyDef bd;
		bd.position.Set(-34.30f + 2.0f * i, 26.0f);
		bd.type = b2_dynamicBody;
		b2Body* sbodydom1 = m_world->CreateBody(&bd);
		b2FixtureDef *fd = new b2FixtureDef;
		fd->density = 1.f;
		fd->shape = new b2PolygonShape;
		fd->shape = &shape;
		sbodydom1->CreateFixture(fd);
    
    
    
		b2PolygonShape shape2;
		shape2.SetAsBox(0.3f, 3.80f);
		b2BodyDef bd2;
		bd2.position.Set(-34.30f + 2.0f * i, 26.0f);
		b2Body* sbodydom2 = m_world->CreateBody(&bd2);
	
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = sbodydom1;
		jointDef.bodyB = sbodydom2;
		jointDef.localAnchorA.Set(0,3.80);
		jointDef.localAnchorB.Set(0,0);
		jointDef.collideConnected = false;
		m_world->CreateJoint(&jointDef);
	    }
     }  
/////////////////////////////
//3 slab for initiation of boll && boll

	{
      b2PolygonShape shape;
      shape.SetAsBox(10.0f, 0.25f);
	
      b2BodyDef bd;
      bd.position.Set(-14.0f, 18.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }
    
        b2Body* sbody;
   {
      b2CircleShape circle;
      circle.m_radius = 1.70;
	
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 1.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(-22.0f, 18.30f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }
	  
     
 ///////////////////////    
	 /* b2PolygonShape shape22;
      shape22.SetAsBox(0.2f, 2.50f);
      b2BodyDef bd3;
      bd3.position.Set(-33.65f, 28.23f);
      b2Body* body21 = m_world->CreateBody(&bd3);
 */
      /*
      b2RevoluteJointDef jointDef;
      jointDef.bodyA = sbodydom;
      jointDef.bodyB = body21;
      jointDef.localAnchorA.Set(0,-2.5);
      jointDef.localAnchorB.Set(0,-1.0);
      jointDef.collideConnected = false;
      m_world->CreateJoint(&jointDef);
	 */
	





/////////////////////////////////////


      
      
    //Another horizontal shelf
   /* {
      b2PolygonShape shape;
      shape.SetAsBox(17.0f, 0.25f, b2Vec2(-20.f,20.f), 0.0f);
	
      b2BodyDef bd;
      bd.position.Set(1.0f, 6.0f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }
*/

    //The pendulum that knocks the dominos off
    
    {
      b2Body* b2;
      {
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 1.5f);
	  
	b2BodyDef bd;
	bd.position.Set(-27.5f, 35.0f);
	b2 = m_world->CreateBody(&bd);
	b2->CreateFixture(&shape, 10.0f);
      }
	
      b2Body* b4;
      {
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 0.25f);
	  
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(-27.0f, 40.0f);
	b4 = m_world->CreateBody(&bd);
	b4->CreateFixture(&shape, 2.0f);
      }
	
      b2RevoluteJointDef jd;
      b2Vec2 anchor;
      anchor.Set(-20.0f, 45.0f);
      jd.Initialize(b2, b4, anchor);
      m_world->CreateJoint(&jd);
    }
    
    //sravi
    
      
    //The train of small spheres
    /*
    {
      b2Body* spherebody;
	
      b2CircleShape circle;
      circle.m_radius = 0.5;
	
      b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 1.0f;
      ballfd.friction = 0.0f;
      ballfd.restitution = 0.0f;
	
      for (int i = 0; i < 10; ++i)
	{
	  b2BodyDef ballbd;
	  ballbd.type = b2_dynamicBody;
	  ballbd.position.Set(-22.2f + i*1.0, 26.6f);
	  spherebody = m_world->CreateBody(&ballbd);
	  spherebody->CreateFixture(&ballfd);
	}
    }
    
    sravi
    */



    //The pulley system
    {
      b2BodyDef *bd = new b2BodyDef;
      bd->type = b2_dynamicBody;
      bd->position.Set(-2,15);
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
      bd->position.Set(5,15);	
      fd1->density = 32.0;	  
      b2Body* box2 = m_world->CreateBody(bd);
      box2->CreateFixture(fd1);

      // The pulley joint
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();
      b2Vec2 worldAnchorOnBody1(-2, 18); // Anchor point on body 1 in world axis
      b2Vec2 worldAnchorOnBody2(5, 18); // Anchor point on body 2 in world axis
      b2Vec2 worldAnchorGround1(-2, 23); // Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(5, 23); // Anchor point for ground 2 in world axis
      float32 ratio = 1.0f; // Define ratio
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);
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

    //The heavy sphere on the platform
 /*   {
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
      ballbd.position.Set(14.0f, 18.0f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
    }
*/

    //The see-saw system at the bottom
    {
      //The triangle wedge
      b2Body* sbody;
      b2PolygonShape poly;
      b2Vec2 vertices[3];
      vertices[0].Set(-1,0);
      vertices[1].Set(1,0);
      vertices[2].Set(0,1.5);
      poly.Set(vertices, 3);
      b2FixtureDef wedgefd;
      wedgefd.shape = &poly;
      wedgefd.density = 10.0f;
      wedgefd.friction = 0.0f;
      wedgefd.restitution = 0.0f;
      b2BodyDef wedgebd;
      wedgebd.position.Set(30.0f, 0.0f);
      sbody = m_world->CreateBody(&wedgebd);
      sbody->CreateFixture(&wedgefd);

      //The plank on top of the wedge
      b2PolygonShape shape;
      shape.SetAsBox(15.0f, 0.2f);
      b2BodyDef bd2;
      bd2.position.Set(30.0f, 1.5f);
      bd2.type = b2_dynamicBody;
      b2Body* body = m_world->CreateBody(&bd2);
      b2FixtureDef *fd2 = new b2FixtureDef;
      fd2->density = 1.f;
      fd2->shape = new b2PolygonShape;
      fd2->shape = &shape;
      body->CreateFixture(fd2);

      b2RevoluteJointDef jd;
      b2Vec2 anchor;
      anchor.Set(30.0f, 1.5f);
      jd.Initialize(sbody, body, anchor);
      m_world->CreateJoint(&jd);

      //The light box on the right side of the see-saw
      b2PolygonShape shape2;
      shape2.SetAsBox(2.0f, 2.0f);
      b2BodyDef bd3;
      bd3.position.Set(40.0f, 2.0f);
      bd3.type = b2_dynamicBody;
      b2Body* body3 = m_world->CreateBody(&bd3);
      b2FixtureDef *fd3 = new b2FixtureDef;
      fd3->density = 0.01f;
      fd3->shape = new b2PolygonShape;
      fd3->shape = &shape2;
      body3->CreateFixture(fd3);
    }
  }

  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
