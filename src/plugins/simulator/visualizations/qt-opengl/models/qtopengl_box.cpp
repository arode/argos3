/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_box.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_box.h"
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/simulator/entity/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/box_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real LED_RADIUS = 0.01f;

   /****************************************/
   /****************************************/

    CQTOpenGLBox::CQTOpenGLBox() :
       m_unVertices(20){

       /* Reserve the needed display lists */
       m_unBaseList = glGenLists(2);
       m_unBodyList = m_unBaseList;
       m_unLEDList = m_unBaseList + 1;

       /* Make body list */
       glNewList(m_unBodyList, GL_COMPILE);
       MakeBody();
       glEndList();

       /* Make LED list */
       glNewList(m_unLEDList, GL_COMPILE);
       MakeLED();
       glEndList();

    }

   /****************************************/
   /****************************************/

   CQTOpenGLBox::~CQTOpenGLBox() {
      glDeleteLists(m_unBaseList, 2);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBox::DrawLEDs(CBoxEntity& c_entity) {
      /* Draw the LEDs */
      GLfloat pfColor[]           = {   0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);
      CLedEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      for(UInt32 i = 0; i < cLEDEquippedEntity.GetAllLeds().size(); ++i) {
         glPushMatrix();
         /* Set the material */
         const CColor& cColor = cLEDEquippedEntity.GetLED(i).GetColor();
         pfColor[0] = cColor.GetRed();
         pfColor[1] = cColor.GetGreen();
         pfColor[2] = cColor.GetBlue();
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
         /* Perform rototranslation */
         const CVector3& cPosition = cLEDEquippedEntity.GetLED(i).GetPosition();
         glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
         /* Draw the LED */
         glCallList(m_unLEDList);
         glPopMatrix();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBox::Draw(const CBoxEntity& c_entity) {
      /* Draw the body */
      glPushMatrix();
      glScalef(c_entity.GetSize().GetX(), c_entity.GetSize().GetY(), c_entity.GetSize().GetZ());
      glCallList(m_unBodyList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   /****************************************/
   /****************************************/

   void CQTOpenGLBox::MakeBody() {
	     /* Since this shape can be stretched,
	         make sure the normal vectors are unit-long */
	      glEnable(GL_NORMALIZE);

	      /* Set the material */
	      const GLfloat pfColor[]     = { 1.0f, 0.0f, 0.0f, 1.0f };
	      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	      const GLfloat pfShininess[] = { 0.0f                   };
	      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
	      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
	      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
	      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);

	      /* Let's start the actual shape */

	      /* This part covers the top and bottom faces (parallel to XY) */
	      glBegin(GL_QUADS);
	      /* Bottom face */
	      glNormal3f(0.0f, 0.0f, -1.0f);
	      glVertex3f( 0.5f,  0.5f, -0.5f);
	      glVertex3f( 0.5f, -0.5f, -0.5f);
	      glVertex3f(-0.5f, -0.5f, -0.5f);
	      glVertex3f(-0.5f,  0.5f, -0.5f);
	      /* Top face */
	      glNormal3f(0.0f, 0.0f, 1.0f);
	      glVertex3f(-0.5f, -0.5f, 0.5f);
	      glVertex3f( 0.5f, -0.5f, 0.5f);
	      glVertex3f( 0.5f,  0.5f, 0.5f);
	      glVertex3f(-0.5f,  0.5f, 0.5f);
	      glEnd();
	      /* This part covers the faces (South, East, North, West) */
	      glBegin(GL_QUADS);
	      /* South face */
	          glNormal3f(0.0f, -1.0f, 0.0f);
	          glVertex3f(-0.5f, -0.5f,  0.5f);
	          glVertex3f(-0.5f, -0.5f, -0.5f);
	          glVertex3f( 0.5f, -0.5f, -0.5f);
	          glVertex3f( 0.5f, -0.5f,  0.5f);
	          /* East face */
	          glNormal3f(1.0f, 0.0f, 0.0f);
	          glVertex3f( 0.5f, -0.5f,  0.5f);
	          glVertex3f( 0.5f, -0.5f, -0.5f);
	          glVertex3f( 0.5f,  0.5f, -0.5f);
	          glVertex3f( 0.5f,  0.5f,  0.5f);
	          /* North face */
	          glNormal3f(0.0f, 1.0f, 0.0f);
	          glVertex3f( 0.5f,  0.5f,  0.5f);
	          glVertex3f( 0.5f,  0.5f, -0.5f);
	          glVertex3f(-0.5f,  0.5f, -0.5f);
	          glVertex3f(-0.5f,  0.5f,  0.5f);
	          /* West face */
	          glNormal3f(-1.0f, 0.0f, 0.0f);
	          glVertex3f(-0.5f,  0.5f,  0.5f);
	          glVertex3f(-0.5f,  0.5f, -0.5f);
	          glVertex3f(-0.5f, -0.5f, -0.5f);
	          glVertex3f(-0.5f, -0.5f,  0.5f);
	      glEnd();
	      /* The shape definitions is finished */

	      /* We don't need it anymore */
	      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLBox::MakeLED() {
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / m_unVertices);

      glBegin(GL_TRIANGLE_STRIP);
      for(CRadians cInclination; cInclination <= CRadians::PI; cInclination += cSlice) {
         for(CRadians cAzimuth; cAzimuth <= CRadians::TWO_PI; cAzimuth += cSlice) {

            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth + cSlice);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth + cSlice);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());

         }
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawBox : public CQTOpenGLOperationDraw {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CBoxEntity& c_entity) {
         c_visualization.DrawPositionalEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   private:
      CQTOpenGLBox m_cModel;
   };

   REGISTER_ENTITY_OPERATION(CQTOpenGLOperationDraw, CQTOpenGLWidget, CQTOpenGLOperationDrawBox, void, CBoxEntity);

   /****************************************/
   /****************************************/

}