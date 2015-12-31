#include <engine/Shader.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath):
		m_program() {
		/* 1: Retrieve the vertex and fragment shaders source codes from the specified paths */
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		// Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try {
			// Open files
			vShaderFile.open(static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "shaders/" << vertexPath).str());
			fShaderFile.open(static_cast<std::ostringstream&> (std::ostringstream().seekp(0) << "shaders/" << fragmentPath).str());
			std::stringstream vShaderStream, fShaderStream;

			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "Error: Shader: File not succesfully read" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();

		/* 2: Compile shaders */
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "Error: Shader: Vertex: Compilation failed\n" << infoLog << std::endl;
		}

		// Fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr << "Error: Shader: Fragment: Compilation failed\n" << infoLog << std::endl;
		}

		/* 3: Shader program */
		m_program = glCreateProgram();
		glAttachShader(m_program, vertex);
		glAttachShader(m_program, fragment);
		glLinkProgram(m_program);
		
		// Print linking errors if any
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_program, 512, NULL, infoLog);
			std::cerr << "Error: Shader: Program: Linking failed\n" << infoLog << std::endl;
		}
		
		// Delete the shaders as they are linked into our program now and are no longer necessery here
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader() {}

	// Use the shader's program
	void Shader::use() const { 
		glUseProgram(m_program); 
	}

}
