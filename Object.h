#ifndef OBJ_H
#define OBJ_H


class Object
{
public:
	Object()
		:status(0),size(glm::vec3(1,1,1))
	{}
	inline void SetPosition(glm::vec3 _position)
	{
		this->position=_position;
	}
	inline void SetSize(glm::vec3 _size)
	{
		this->size=_size;
	}
	inline void SetRotation(glm::vec3 _rotation)
	{
		this->rotation=_rotation;
	}
	inline void SetDirection(glm::vec3 _direction)
	{
		this->direction=_direction;
	}
	inline void SetSpeed(float _speed)
	{
        this->speed=_speed;
	}
	inline void AddTexture(GLuint _texture)
	{
		char _status= status & 15;
		if(_status>6) {std::cerr<<"Object: too many textures\n"; return;}
		this->textures[(uint16_t)_status]=_texture;
		++_status;
		status=_status;
	}
	inline void SetShader(std::string vertex_shader,std::string geometry_shader, std::string fragment_shader)
	{
		shader.Create();
		shader.LoadFromFile(vertex_shader,GL_VERTEX_SHADER);
		if(geometry_shader!="" and geometry_shader!="NULL" and geometry_shader!="0")
			shader.LoadFromFile(geometry_shader,GL_GEOMETRY_SHADER);
		shader.LoadFromFile(fragment_shader,GL_FRAGMENT_SHADER);
		shader.Link();
	}
	inline void SetMesh(Mesh *_mesh)
	{
		this->mesh=_mesh;
	}
	inline void SetAlive()
	{
		status |= 16;
	}
	inline void SetSolide()
	{
		status |= 32;
	}
	inline void SetDefaultRendering()
	{
		status |= 64;
	}
	inline void SetDead()
	{
		status &=239;
	}
	inline void SetGhost()
	{
		status &=223;
	}
	inline void SetCustomRendering()
	{
		status &=191;
	}
	inline glm::vec3 GetPosition()
	{
		return position;
	}
	inline glm::vec3 GetSize()
	{
		return size;
	}
	inline glm::vec3 GetRotation()
	{
		return rotation;
	}
	inline glm::vec3 GetDirection()
	{
		return direction;
	}
	inline float GetSpeed()
	{
	    return this->speed;
	}
	inline uint16_t GetNr_Texture()
	{
		return uint16_t( status & 15);
	}
	inline bool GetAlive()
	{
		return bool(status | 16);
	}
	inline bool GetSolide()
	{
		return bool(status | 32);
	}
	inline bool GetCustomRendering()
	{
		return bool(status | 64);
	}
	void Render(glm::mat4 *projection,glm::mat4 *view)
	{
		shader.Use();
		uint16_t nr_textures=(uint16_t)GetNr_Texture();
        for(uint16_t i=0;i<nr_textures;++i)
        {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D,textures[i]);
            string s="ourTexture";
            s+=char(i+48);
            glUniform1i(glGetUniformLocation(shader.Program, s.c_str()), i);
        }
        shader.SetMatrix4("projection",*projection);
        shader.SetMatrix4("view",*view);
        glm::mat4 model;
        model=glm::rotate(model,rotation.x,glm::vec3(1.0,0.0,0.0));
        model=glm::rotate(model,rotation.y,glm::vec3(0.0,1.0,0.0));
        model=glm::rotate(model,rotation.z,glm::vec3(0.0,0.0,1.0));
        model=glm::scale(model,size);
       	model=glm::translate(model,position);
        shader.SetMatrix4("model",model);
        mesh->Draw();
    }
	void Update()
	{
		position=position+direction*speed;
	}
	~Object(){}
private:
	glm::vec3 position;
	glm::vec3 size;
	glm::vec3 rotation;
	glm::vec3 direction;
	GLuint textures[8];
	Shader shader;
	Mesh *mesh;
	float speed;
	///Coliter colider;
	char status;
};
#endif ///OBJ_H
