#ifndef SKYBOX_H_H
#define SKYBOX_H_H

class Skybox
{
public:

    Skybox()
        :Time(0),TimeUnit(0),SpeedR(0), BoolDay(false), BoolNight(false)
    {
    	skyboxVertices = new GLfloat[24];
    	skyboxIndices = new uint8_t[36];
    	GLfloat _skyboxVertices[] = {
            1.000000, -1.000000, -1.000000,
            1.000000, -1.000000, 1.000000,
            -1.000000, -1.000000, 1.000000,
            -1.000000, -1.000000, -1.000000,
            1.000000, 1.000000, -0.999999,
            0.999999, 1.000000, 1.000001,
            -1.000000, 1.000000, 1.000000,
            -1.000000, 1.000000, -1.000000
    	};
    	 uint8_t _skyboxIndices[]={
    								0, 1, 3,
									4, 7, 5,
									0, 4, 1,
									1, 5, 2,
									2, 6, 3,
									4, 0, 7,
									1, 2, 3,
									7, 6, 5,
									4, 5, 1,
									5, 6, 2,
									6, 7, 3,
									0, 3, 7
								};

		ptrcpy(skyboxVertices,_skyboxVertices,24*sizeof(GLfloat));
		ptrcpy(skyboxIndices,_skyboxIndices,36);
        glGenVertexArrays(1,&VAO);
    	glGenBuffers(1,&VBO);
    	glGenBuffers(1,&EBO);
    	glBindVertexArray(VAO);
    	glBindBuffer(GL_ARRAY_BUFFER,VBO);
    	glBufferData(GL_ARRAY_BUFFER,24*sizeof(GLfloat),skyboxVertices,GL_DYNAMIC_DRAW);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER,36,skyboxIndices,GL_DYNAMIC_DRAW);
    	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLvoid*)0);
    	glEnableVertexAttribArray(0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindVertexArray(0);
    }
    void LoadShader(std::string vertex_shader,std::string fragment_shader)
    {
        skyboxShader.Create();
        skyboxShader.LoadFromFile(vertex_shader,GL_VERTEX_SHADER);
        skyboxShader.LoadFromFile(fragment_shader,GL_FRAGMENT_SHADER);
        skyboxShader.Link();
    }
    void SetDayTexture(std::string dir_name)
    {
        if(glIsTexture(DayTexture) == GL_FALSE)
        {
            glDeleteTextures((GLsizei)1,&DayTexture);
        }
        BoolDay=true;
        LoadTexture(dir_name,&DayTexture);
    }
    void SetNightTexture(std::string dir_name)
    {
        if(glIsTexture(NightTexture) == GL_FALSE)
        {
            glDeleteTextures((GLsizei)1,&NightTexture);
        }
        BoolNight=true;
        LoadTexture(dir_name,&NightTexture);
    }
    void UnsetDayTexture()
    {

        if(!BoolNight) std::cerr<<"Skybox: Warning: DayTexture and NightTexture are unset\n\n";
        BoolDay=false;
        Time=1;
        glDeleteTextures(1,&DayTexture);
    }
    void UnsetNightTexture()
    {
        if(!DayTexture) std::cerr<<"Skybox: Warning: DayTexture and NightTexture are unset\n\n";
        BoolNight=false;
        Time=0;
        glDeleteTextures(1,&NightTexture);
    }
    inline void SetTime(float time)
    {
    	this->Time=time;
    }
    inline void SetTimeUnit(float unit)
    {
        this->TimeUnit = unit;
    }
    inline void SetSpeedR(float unit)
    {
        this->SpeedR = unit;
    }
    inline void SetProjection(glm::mat4 _projection)
    {
        projection=_projection;
    }
    void Draw(glm::mat4 *view)
    {
        glDepthMask(GL_FALSE);// Remember to turn depth writing off
        skyboxShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, DayTexture);
        glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox_day"),0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, NightTexture);
        glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox_night"),1);
        skyboxShader.SetMatrix4("projection",projection);
        skyboxShader.SetMatrix4("view",*view);
        skyboxShader.SetMatrix4("model",model);
        skyboxShader.SetFloat("time",Time);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_BYTE,0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glDepthMask(GL_TRUE);
        model=glm::rotate(model,SpeedR,glm::vec3(0.0,1.0,0.0));
        ///Update time
        Time+=TimeUnit;
        
    }
    virtual ~Skybox()
    {
        delete[] skyboxVertices;
        delete[] skyboxIndices;
    }
private:
        bool BoolNight;
       	bool BoolDay;
    
        GLuint VBO,VAO,EBO;
        GLuint DayTexture;
        GLuint NightTexture;
       	float Time,TimeUnit;
       	float SpeedR;
        GLfloat *skyboxVertices;
        uint8_t *skyboxIndices;
        glm::mat4 model;
        glm::mat4 projection;
        Shader skyboxShader;


        GLuint LoadTexture(std::string dir_name ,GLuint *texture)
        {
        std::string texture_faces[6];
        texture_faces[0]=dir_name + "/r.jpg";//right
        texture_faces[1]=dir_name + "/l.jpg";//left
        texture_faces[2]=dir_name + "/u.jpg";//up
        texture_faces[3]=dir_name + "/d.jpg";//down
        texture_faces[4]=dir_name + "/b.jpg";//back
        texture_faces[5]=dir_name + "/f.jpg";//front

        glGenTextures(1, texture);
        glActiveTexture(GL_TEXTURE0);
        int width,height;
        unsigned char* image;

        glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
        for(GLuint i = 0; i < 6; i++)
        {
            if(!FileExist(texture_faces[i]))
            {
                std::cerr<<"Skybox: File "<<texture_faces[i]<<" doesn't exist!\n";
                exit(EXIT_FAILURE);
            }
            image = SOIL_load_image(texture_faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
            );
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
};
#endif // SKYBOX_H_H
