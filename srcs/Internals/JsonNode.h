#pragma once

class JsonBuffer;
class JsonWriter;
class JsonNodeIterator;

class JsonNode
{
    friend class JsonNodeIterator;

    enum JsonNodeType
    {
        JSON_UNDEFINED,
        JSON_NULL,
        JSON_ARRAY,
        JSON_OBJECT,
        JSON_KEY_VALUE,
        JSON_BOOLEAN,
        JSON_STRING,
        JSON_LONG,
        JSON_DOUBLE_0_DECIMALS,
        JSON_DOUBLE_1_DECIMAL,
        JSON_DOUBLE_2_DECIMALS,
        // etc.
    };

    union JsonNodeContent
    {
        bool asBoolean;
        double asDouble;
        long asInteger;
        const char* asString;

        struct
        {
            const char* key;
            JsonNode* value;
        } asKey;

        struct
        {
            JsonNode* child;
            JsonBuffer* buffer;
        } asContainer;

        struct
        {
            JsonNode* target;
        } asProxy;

    };

public:
    JsonNode()
        : type(JSON_UNDEFINED), next(0)
    {
        
    }
    
    void writeTo(JsonWriter&); // TODO: <- move in JsonNodeSerializer

    void setAsArray(JsonBuffer* buffer)
    {
        type = JSON_ARRAY;
        content.asContainer.child = 0;
        content.asContainer.buffer = buffer;
    }

    void setAsBoolean(bool value)
    {
        type = JSON_BOOLEAN;
        content.asBoolean = value;
    }

    void setAsLong(int value)
    {
        type = JSON_LONG;
        content.asInteger = value;
    }

    void setAsString(char const* value)
    {
        type = JSON_STRING;
        content.asString = value;
    }

    void setAsDouble(double value, int decimals)
    {
        type = (JsonNodeType) (JSON_DOUBLE_0_DECIMALS + decimals);
        content.asDouble = value;
    }

    void setAsObject(JsonBuffer* buffer)
    {
        type = JSON_OBJECT;
        content.asContainer.child = 0;
        content.asContainer.buffer = buffer;
    }

    void setAsObjectKeyValue(const char* key, JsonNode* value)
    {
        type = JSON_KEY_VALUE;
        content.asKey.key = key;
        content.asKey.value = value;
    }

    bool getAsBoolean()
    {
        return type == JSON_BOOLEAN ? content.asBoolean : false;
    }

    double getAsDouble()
    {
        return type > JSON_DOUBLE_0_DECIMALS ? content.asDouble : 0;
    }

    long getAsInteger()
    {
        return type == JSON_LONG ? content.asInteger : 0;
    }

    const char* getAsString()
    {
        return type == JSON_STRING ? content.asString : 0;
    }

    JsonBuffer* getContainerBuffer()
    {
        return type == JSON_ARRAY || type == JSON_OBJECT ? content.asContainer.buffer : 0;
    }

    JsonNode* getContainerChild()
    {
        return type == JSON_ARRAY || type == JSON_OBJECT ? content.asContainer.child : 0;
    }

    const char* getAsObjectKey()
    {
        return type == JSON_KEY_VALUE ? content.asKey.key : 0;
    }

    JsonNode* getAsObjectValue()
    {
        return type == JSON_KEY_VALUE ? content.asKey.value : 0;
    }

    void addChildToContainer(JsonNode* childToAdd);

    void removeChildFromContainer(JsonNode* childToRemove);

private:
    JsonNode* next;
    JsonNodeContent content;
    JsonNodeType type;

    inline void writeArrayTo(JsonWriter&);// TODO: <- move in JsonNodeSerializer
    inline void writeObjectTo(JsonWriter&);// TODO: <- move in JsonNodeSerializer
};