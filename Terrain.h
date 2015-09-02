#ifndef TERRAIN_H_H
#define TERRAIN_H_H

struct Terrain_Header
{
    uint16_t vertices_on_x;
    uint16_t vertices_on_z;
    float distance_unit;
    uint32_t indices_nr;
};

class Terrain
{
public:
    Terrain_Header _Terrain_Header;
    float *vertices;
    uint32_t *indices;
    GLuint VBO,VAO,EBO;
    Shader shader;
    uint8_t nr_textures;
    glm::mat4 projection;
    GLuint *Textures;
    Terrain()
        : nr_textures(0)
    {
        Textures=new GLuint[8];
        shader.Create();
        std::cerr<<"  Init Terrain...\n";
    }
    inline void SetProjection(glm::mat4 *_projection)
    {
        projection=*_projection;
    }
    void LoadTerrain(std::string file_name)
    {
        std::fstream file;
        file.open(&file_name[0],std::ios::in | std::ios::binary);
        if(file.fail())
        {
            std::cerr<<" Terrain: File "<<file_name<<" doesn't exist!\n";
            return;
        }
        file.read((char*)&_Terrain_Header,sizeof(Terrain_Header));
        vertices=new float[3*_Terrain_Header.vertices_on_x*_Terrain_Header.vertices_on_z];
        indices=new uint32_t[_Terrain_Header.indices_nr];
        file.read((char*)vertices,12*_Terrain_Header.vertices_on_x*_Terrain_Header.vertices_on_z);
        file.read((char*)indices,_Terrain_Header.indices_nr*4);
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
    	glGenBuffers(1,&VBO);
    	glGenBuffers(1,&EBO);
    	glBindBuffer(GL_ARRAY_BUFFER,VBO);
    	glBufferData(GL_ARRAY_BUFFER,12*_Terrain_Header.vertices_on_x*_Terrain_Header.vertices_on_z,vertices,GL_DYNAMIC_DRAW);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER,_Terrain_Header.indices_nr*4,indices,GL_DYNAMIC_DRAW);
    	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,(GLvoid*)0);
    	glEnableVertexAttribArray(0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindVertexArray(0);
    }
    void LoadTexture(std::string file_name)
    {
        if(!FileExist(file_name))
        {
            std::cerr<<"Terrain: File "<<file_name<<" doesn't exist!\n";
            std::exit(EXIT_FAILURE);
        }
        if(nr_textures>=8)
        {
            std::cerr<<"Terrain: too many textures\n";
            std::exit(EXIT_FAILURE);
        }
        GLuint texture;
        int height,width;
	    unsigned char *image;
		image = SOIL_load_image(&file_name[0],&width,&height,0,SOIL_LOAD_RGB);
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D,texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D,0);
        Textures[nr_textures++]=texture;
    }
    void LoadShaders(std::string vertexShader, std::string geometryShader, std::string fragmentShader)
    {
        shader.LoadFromFile(vertexShader,GL_VERTEX_SHADER);
        shader.LoadFromFile(fragmentShader,GL_FRAGMENT_SHADER);
        if(geometryShader!= "" and geometryShader!="NULL" and geometryShader!="null" and geometryShader!="0")
        {
            shader.LoadFromFile(geometryShader,GL_GEOMETRY_SHADER);
        }
        shader.Link();
    }
    inline bool Inside(float z, float x)
    {
        return z>=0 and z<=(_Terrain_Header.vertices_on_z-1)*_Terrain_Header.distance_unit and
               x>=0 and x<=(_Terrain_Header.vertices_on_x-1)*_Terrain_Header.distance_unit;
    }
    inline float GetHight(float z,float x)
    {
        if(Inside(z,x))
        {
            return vertices[(uint16_t(z/_Terrain_Header.distance_unit)*_Terrain_Header.vertices_on_x+uint16_t(x/_Terrain_Header.distance_unit))*3+1];
        }
    }
    inline uint16_t GetVertices_on_X()
    {
        return _Terrain_Header.vertices_on_x;
    }
    inline uint16_t GetVertices_on_Z()
    {
        return _Terrain_Header.vertices_on_z;
    }
    void Info()
    {
        std::cerr<<"\n\n###########################\n";
        std::cerr<<"# Vertices on x: "<<_Terrain_Header.vertices_on_x<<"\n";
        std::cerr<<"# Vertices on z: "<<_Terrain_Header.vertices_on_z<<"\n";
        std::cerr<<"# Distance unit: "<<_Terrain_Header.distance_unit<<"\n";
        std::cerr<<"# Indices nr: "<<_Terrain_Header.indices_nr<<"\n";
        std::cerr<<"# Nr. Textures: "<<uint16_t(nr_textures)<<"\n";
        std::cerr<<"############################\n\n";
    }
    void Draw(glm::mat4 *view)
    {
        shader.Use();
        for(uint16_t i=0;i<nr_textures;++i)
        {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D,Textures[i]);
            string s="ourTexture";
            s+=char(i+48);
            glUniform1i(glGetUniformLocation(shader.Program, s.c_str()), i);
        }
        shader.SetMatrix4("projection",projection);
        shader.SetMatrix4("view",*view);
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, _Terrain_Header.indices_nr , GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    virtual ~Terrain()
    {
        glDeleteTextures((GLsizei)nr_textures,Textures);
        delete[] vertices;
    }
};


#endif // TERRAIN_H_H
