#version 330 core
in vec3 out_color_vertex;
in vec3 Frag_pos;
in vec3 Normal;
out vec4 color;



uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 view_pos;

void main()
{
    
    float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * light_color;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light_pos - Frag_pos);
    float diff = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse = diff * light_color;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(view_pos - Frag_pos);
    vec3 reflectDir = reflect(-light_dir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;  
        
    vec3 result = (ambient + diffuse + specular) * out_color_vertex;
    
    color = vec4(result, 1.0);
}