void pyramid(){
	glBegin(GL_TRIANGLES);
		//front
		glColor3f(1,1,1); 
		glVertex3f(-2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		//right
		
		glVertex3f(2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,-2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		//back
		
		glVertex3f(-2.0f,-2.0f,-2.0f);
		glVertex3f(2.0f,-2.0f,-2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		//left
		
		glVertex3f(-2.0f,-2.0f,2.0f);
		glVertex3f(-2.0f,-2.0f,-2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);
	glEnd();

	glBegin(GL_QUADS);
		//bottom
	    glColor3f(0.0f,0.0f,0.0f); //black
		glVertex3f(-2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,-2.0f);
		glVertex3f(-2.0f,-2.0f,-2.0f);

		
	glEnd();
}
void line(){
	glBegin(GL_LINES);
	glLineWidth(10);	
	//front
		glColor3f(0,0,0); 
		glVertex3f(-2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		//right
		
		glVertex3f(2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,-2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		//back
		
		glVertex3f(-2.0f,-2.0f,-2.0f);
		glVertex3f(2.0f,-2.0f,-2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		//left
		
		glVertex3f(-2.0f,-2.0f,2.0f);
		glVertex3f(-2.0f,-2.0f,-2.0f);
		glVertex3f(0.0f,-0.3f,0.0f);

		
	glEnd();

	glBegin(GL_LINES);
	glLineWidth(3);	
	//bottom
	    glColor3f(1.0f,1.0f,0.0f); //black
		glVertex3f(-2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,2.0f);
		glVertex3f(2.0f,-2.0f,-2.0f);
		glVertex3f(-2.0f,-2.0f,-2.0f);

		
	glEnd();	
}
