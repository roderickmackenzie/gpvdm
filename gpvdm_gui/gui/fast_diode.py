	def paint_resistor(self,o):
		glPushMatrix()
		glTranslatef(o.xyz.x,o.xyz.y,o.xyz.z)

		glLineWidth(2)
		self.set_color(o)
		glBegin(GL_LINES)
		glVertex3f(0.0, 0.0, 0.0)
		glVertex3f(o.dxyz.x, o.dxyz.y, o.dxyz.z)
		glEnd()


		glLineWidth(5)
		glBegin(GL_LINES)
		glVertex3f(o.dxyz.x*0.3, o.dxyz.y*0.3, o.dxyz.z*0.3)
		glVertex3f(o.dxyz.x*0.7, o.dxyz.y*0.7, o.dxyz.z*0.7)
		glEnd()

		glPopMatrix()

	def paint_diode(self,o):
		diode_max=0.7
		glPushMatrix()
		glTranslatef(o.xyz.x,o.xyz.y,o.xyz.z)
		glLineWidth(2)
		self.set_color(o)
		glBegin(GL_LINES)
		glVertex3f(0.0, 0.0, 0.0)
		glVertex3f(o.dxyz.x, o.dxyz.y, o.dxyz.z)
		glEnd()


		glLineWidth(2)
		glBegin(GL_LINES)
		#arrow btm
		glVertex3f(-0.1, o.dxyz.y*0.3, 0.0)
		glVertex3f(0.1, o.dxyz.y*0.3, 0.0)
		#bar top
		glVertex3f(-0.1, o.dxyz.y*diode_max, 0.0)
		glVertex3f(0.1, o.dxyz.y*diode_max, 0.0)

		#arrow left
		glVertex3f(-0.1, o.dxyz.y*0.3, 0.0)
		glVertex3f(0.0, o.dxyz.y*diode_max, 0.0)

		#arrow right
		glVertex3f(+0.1, o.dxyz.y*0.3, 0.0)
		glVertex3f(0.0, o.dxyz.y*diode_max, 0.0)

		glEnd()
		glPopMatrix()
