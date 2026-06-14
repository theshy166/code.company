#ifndef IOT_THING_H_
#define IOT_THING_H_

#include <stdbool.h>

typedef enum iot_value_type {
    kValueTypeBoolean,
    kValueTypeNumber,
    kValueTypeString
} iot_value_type;

typedef struct iot_property {
    char* name;
    char* description;
    iot_value_type type;
    bool (*boolean_getter)(void*object);
    int (*number_getter)(void*object);
    char* (*string_getter)(void*object);
} iot_property;

typedef struct iot_property_list {
    iot_property** properties;
    int count;
    int capacity;
} iot_property_list;

typedef struct iot_parameter {
    char* name;
    char* description;
    iot_value_type type;
    bool required;
    bool boolean_value;
    int number_value;
    char* string_value;
} iot_parameter;

typedef struct iot_parameter_list {
    iot_parameter** parameters;
    int count;
    int capacity;
} iot_parameter_list;

typedef struct iot_method {
    char* name;
    char* description;
    iot_parameter_list* parameters;
    void (*callback)(void*object, iot_parameter_list*);
} iot_method;

typedef struct iot_method_list {
    iot_method** methods;
    int count;
    int capacity;
} iot_method_list;

typedef struct iot_thing_t {
    char* name;
    char* description;
    iot_property_list* properties;
    iot_method_list* methods;
    void * object; //实际实现函数和属性的指针； 由具体的实现类提供
    void (*pf_free_object)(void * object); // 用于释放 object 的函数指针, 可为NULL， 如果为NULL，表示object不需要在这里释放
} iot_thing_t;

// iot_property export functions

// iot_property_list export functions
void PropertyList_addBooleanProperty(iot_property_list* list, const char* name, const char* description, bool (*getter)(void*object));
void PropertyList_addNumberProperty(iot_property_list* list, const char* name, const char* description, int (*getter)(void*object));
void PropertyList_addStringProperty(iot_property_list* list, const char* name, const char* description, char* (*getter)(void*object));

// iot_parameter export functions
//after parameter created, add to parameter list, the parameter will be freed by parameter list
iot_parameter* Parameter_create(const char* name, const char* description, iot_value_type type, bool required);
//parameter getters is invoked by the method callback;
//  while setters is invoked by the iot_thing_invoke internally;
bool Parameter_getBoolean(iot_parameter* param);
int Parameter_getNumber(iot_parameter* param);
const char* Parameter_getString(iot_parameter* param);

// iot_parameter_list export functions
iot_parameter_list* ParameterList_create(); // after created and add to method, the list will be freed by method
void ParameterList_addParameter(iot_parameter_list* list, iot_parameter* param);
iot_parameter* ParameterList_get(iot_parameter_list* list, const char* name);

// iot_method export functions

// iot_method_list export functions
void MethodList_addMethod(iot_method_list* list, const char* name, const char* description, 
                         iot_parameter_list* params, void (*callback)(void*, iot_parameter_list*));


// iot_thing_t functions
// iot_thing_create: properties and methods are initialized with empty lists
iot_thing_t* iot_thing_create(const char* name, const char* description, void * object, void (*pf_free_object)(void * object));
void iot_thing_destroy(iot_thing_t* thing);
const char* iot_thing_get_name(iot_thing_t* thing);
//descriptor: json format: contains properties and methods descriptors
char* iot_thing_get_descriptor(iot_thing_t* thing); 
char* iot_thing_get_state(iot_thing_t* thing);

void iot_thing_invoke(iot_thing_t* thing, const char * command); //command is json format

iot_property_list* iot_thing_get_properties(iot_thing_t* thing);
iot_method_list* iot_thing_get_methods(iot_thing_t* thing);


#endif // IOT_THING_H_