cl_program createProgramFromFile(cl_context context, const char* source_file){
    FILE* program_handle = fopen(source_file, "r");
    fseek(program_handle, 0, SEEK_END);
    size_t program_size = ftell(program_handle);
    rewind(program_handle);
    char* program_buffer = (char*)malloc(program_size + 1);
    fread(program_buffer, sizeof(char), program_size, program_handle);
    program_buffer[program_size] = '\0';
    cl_program program = clCreateProgramWithSource(
        context,1,(const char**)&program_buffer,&program_size,NULL);
    fclose(program_handle);
    free(program_buffer);
    return program;
};
