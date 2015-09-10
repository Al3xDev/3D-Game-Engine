#ifndef MESH_H
#define MESH_H
class BaseMesh
{
public:
	uint32_t NR_vertices_indices;
	GLuint VAO;
	virtual void Draw()=0;
};
class ArraysMesh : public BaseMesh
{
public:
	inline void Draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,NR_vertices_indices);
		glBindVertexArray(VAO);
	}

};
class ElementsMesh : public BaseMesh
{
public:
	inline void Draw()
	{
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,NR_vertices_indices,GL_UNSIGNED_INT,0);
        glBindVertexArray(VAO);
	}
};
class Mesh
{
public:
    void LoadObj(std::string ObjFileName)
	{
	    FileName=ObjFileName;
		bool bool_texture=false,bool_normal=false;
		uint32_t Nr_Vertices=0;
		GLuint VBO, EBO;
	   	std::ifstream ObjFile(ObjFileName.c_str());
		std::vector<float> temp_vertices;
		std::vector<float> temp_textureCoord;
		std::vector<float> temp_normals;
		std::vector<float> final_vertices;
		std::vector<uint32_t> indices;
		if(ObjFile.fail())
		{
			std::cerr<<"Mesh: File "<<ObjFileName<<" doesn't exist\n\n";
			exit(EXIT_FAILURE);
		}
		std::string _line;
		char aux;
		float x,y,z;
		while(getline(ObjFile,_line))
		{
			if(_line[0]=='v')
			{
				if(_line[1]==' ')///vertices
				{
					sscanf(&_line[2],"%f %f %f",&x,&y,&z);
					temp_vertices.push_back(x);
					temp_vertices.push_back(y);
					temp_vertices.push_back(z);
				}
				else
				{
					if(_line[1]=='t')///textureCoord
					{
					    bool_texture=true;
                        sscanf(&_line[2],"%f %f",&x,&y);
                        temp_textureCoord.push_back(x);
                        temp_textureCoord.push_back(y);
					}
					else///normals 'vn'
					{
					    bool_normal=true;
                        sscanf(&_line[2],"%f %f %f",&x,&y,&z);
                        temp_normals.push_back(x);
                        temp_normals.push_back(y);
                        temp_normals.push_back(z);
					}
				}
			}
			else
            {
                if(_line[0]=='f')
                {
                    Nr_Vertices+=3;
                    if(!bool_texture and !bool_normal)
                    {
                        int v1,v2,v3;
                        sscanf(&_line[1],"%d %d %d",&v1,&v2,&v3);
                        indices.push_back(v1-1);
                        indices.push_back(v2-1);
                        indices.push_back(v3-1);
                    }
                    else
                    {
                    	if(bool_texture and !bool_normal)
                    	{
                    		int v1,v2,v3,t1,t2,t3;
	                        sscanf(&_line[1],"%d%c%d %d%c%d %d%c%d",&v1,&aux,&t1,&v2,&aux,&t2,&v3,&aux,&t3);
	                        ///vertex 1
		                    final_vertices.push_back(temp_vertices[(v1-1)*3]);
	                        final_vertices.push_back(temp_vertices[(v1-1)*3+1]);
	                        final_vertices.push_back(temp_vertices[(v1-1)*3+2]);
	                        final_vertices.push_back(temp_textureCoord[(t1-1)*2]);
	                        final_vertices.push_back(temp_textureCoord[(t1-1)*2+1]);
	                        ///vertex 2
	                        final_vertices.push_back(temp_vertices[(v2-1)*3]);
	                        final_vertices.push_back(temp_vertices[(v2-1)*3+1]);
	                        final_vertices.push_back(temp_vertices[(v2-1)*3+2]);
	                        final_vertices.push_back(temp_textureCoord[(t2-1)*2]);
	                        final_vertices.push_back(temp_textureCoord[(t2-1)*2+1]);
	                        ///vertex 3
	                        final_vertices.push_back(temp_vertices[(v3-1)*3]);
	                        final_vertices.push_back(temp_vertices[(v3-1)*3+1]);
	                        final_vertices.push_back(temp_vertices[(v3-1)*3+2]);
	                        final_vertices.push_back(temp_textureCoord[(t3-1)*2]);
	                        final_vertices.push_back(temp_textureCoord[(t3-1)*2+1]);
                    	}
                    	else
                    	{
                    		if(bool_texture & bool_texture)
                    		{
                    			int v1,v2,v3,t1,t2,t3,n1,n2,n3;
		                        sscanf(&_line[1],"%d/%d/%d %d/%d/%d %d/%d/%d",&v1,&t1,&n1,&v2,&t2,&n2,&v3,&t3,&n3);
		                        ///vertex 1
			                    final_vertices.push_back(temp_vertices[(v1-1)*3]);
		                        final_vertices.push_back(temp_vertices[(v1-1)*3+1]);
		                        final_vertices.push_back(temp_vertices[(v1-1)*3+2]);
		                        final_vertices.push_back(temp_textureCoord[(t1-1)*2]);
		                        final_vertices.push_back(temp_textureCoord[(t1-1)*2+1]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+1);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+2);
		                        ///vertex 2
		                        final_vertices.push_back(temp_vertices[(v2-1)*3]);
		                        final_vertices.push_back(temp_vertices[(v2-1)*3+1]);
		                        final_vertices.push_back(temp_vertices[(v2-1)*3+2]);
		                        final_vertices.push_back(temp_textureCoord[(t2-1)*2]);
		                        final_vertices.push_back(temp_textureCoord[(t2-1)*2+1]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+1);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+2);
		                        ///vertex 3
		                        final_vertices.push_back(temp_vertices[(v3-1)*3]);
		                        final_vertices.push_back(temp_vertices[(v3-1)*3+1]);
		                        final_vertices.push_back(temp_vertices[(v3-1)*3+2]);
		                        final_vertices.push_back(temp_textureCoord[(t3-1)*2]);
		                        final_vertices.push_back(temp_textureCoord[(t3-1)*2+1]);
 								final_vertices.push_back(temp_normals[(n1-1)*3]);
 								final_vertices.push_back(temp_normals[(n1-1)*3]+1);
 								final_vertices.push_back(temp_normals[(n1-1)*3]+2);
                    		}
                    		else
                    		{
                    			if(bool_normal and !bool_texture)
                    			{
                    			int v1,v2,v3,n1,n2,n3;
		                        sscanf(&_line[1],"%d//%d %d//%d %d//%d",&v1,&n1,&v2,&n2,&v3,&n3);
		                        ///vertex 1
			                    final_vertices.push_back(temp_vertices[(v1-1)*3]);
		                        final_vertices.push_back(temp_vertices[(v1-1)*3+1]);
		                        final_vertices.push_back(temp_vertices[(v1-1)*3+2]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+1);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+2);
		                        ///vertex 2
		                        final_vertices.push_back(temp_vertices[(v2-1)*3]);
		                        final_vertices.push_back(temp_vertices[(v2-1)*3+1]);
		                        final_vertices.push_back(temp_vertices[(v2-1)*3+2]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+1);
		                        final_vertices.push_back(temp_normals[(n1-1)*3]+2);
		                        ///vertex 3
		                        final_vertices.push_back(temp_vertices[(v3-1)*3]);
		                        final_vertices.push_back(temp_vertices[(v3-1)*3+1]);
		                        final_vertices.push_back(temp_vertices[(v3-1)*3+2]);
 								final_vertices.push_back(temp_normals[(n1-1)*3]);
 								final_vertices.push_back(temp_normals[(n1-1)*3]+1);
 								final_vertices.push_back(temp_normals[(n1-1)*3]+2);
                    			}
                    		}
                    	}
                    }
                }
            }
		}
		if(!bool_texture and !bool_normal)
		{
		    std::cerr<<"\n    Mesh: File "<<FileName<<": EMODEL => use glDrawElements()\n\n";
			mesh=new ElementsMesh;
			mesh->NR_vertices_indices=indices.size();
            glGenVertexArrays(1, &mesh->VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(mesh->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, temp_vertices.size()*4, &temp_vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*4, &indices[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
		}
		else
        {
            std::cerr<<"\n    Mesh: File "<<FileName<<": AMODEL => use glDrawArrays()\n\n";
            uint16_t size_=(3+2*bool_texture+3*bool_normal)*sizeof(float);
            mesh=new ArraysMesh;
            mesh->NR_vertices_indices=final_vertices.size()/size_*4;
            glGenBuffers(1,&VBO);
            glGenVertexArrays(1,&mesh->VAO);
            glBindVertexArray(mesh->VAO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER,final_vertices.size()*sizeof(GLfloat),&final_vertices[0],GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,size_,(GLvoid*)0);
            if(bool_texture and !bool_normal)
            {
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,size_,(GLvoid*)(sizeof(float)*3));
            }
            else
                if(bool_texture and bool_normal)
                {
                    glEnableVertexAttribArray(2);
                    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,size_,(GLvoid*)(sizeof(float)*5));
                }
                else
                {
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,size_,(GLvoid*)(sizeof(float)*3));
                }
            glBindVertexArray(0);
        }

	}
	void LoadAModel(std::string AModelFileName)
	{
	    GLuint VBO;
	    FileName=AModelFileName;
        std::fstream AModelFile;
        AModelFile.open(AModelFileName.c_str(),std::ios::in | std::ios::binary);
        if(AModelFile.fail())
        {
            std::cerr<<"Mesh: File "<<AModelFileName<<" doesn't exist\n\n";
			exit(EXIT_FAILURE);
        }
        mesh=new ArraysMesh;
        char bool_textureCoord_normal;
        AModelFile>>mesh->NR_vertices_indices;
        AModelFile>>bool_textureCoord_normal;
        float *vertices;
        uint16_t size_vertex=0;
        if(bool_textureCoord_normal==1)///textureCoord
        {
            vertices=new float[mesh->NR_vertices_indices*5];
            size_vertex=5;
        }
        else
        {
            if(bool_textureCoord_normal==2)///normals
            {
                vertices=new float[mesh->NR_vertices_indices*6];
                size_vertex=6;
            }
            else
            {
                if(bool_textureCoord_normal==3)///textureCoord and normals
                {
                    vertices=new float[mesh->NR_vertices_indices*8];
                    size_vertex=8;
                }
                else
                {
                    std::cerr<<"Mesh: "<<FileName<<": corupt file\n\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        AModelFile.read((char*)vertices,mesh->NR_vertices_indices*size_vertex);

        glGenBuffers(1,&VBO);
        glGenVertexArrays(1,&mesh->VAO);
        glBindVertexArray(mesh->VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,mesh->NR_vertices_indices*size_vertex,vertices,GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,size_vertex,(GLvoid*)0);
        if(bool_textureCoord_normal==1)
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,size_vertex,(GLvoid*)(sizeof(float)*3));
        }
        else
            if(bool_textureCoord_normal==3)
            {
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,size_vertex,(GLvoid*)(sizeof(float)*5));
            }
            else
            {
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,size_vertex,(GLvoid*)(sizeof(float)*3));
            }
        glBindVertexArray(0);
	}
	void LoadEModel(std::string EModelFileName)
	{
        std::cerr<<"Mesh: Function = 0\n\n";
	}
	inline void Draw()
	{
		mesh->Draw();
	}
	~Mesh(){}

private:
    BaseMesh *mesh;
	std::string FileName;
};
#endif
