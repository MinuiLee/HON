#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
/*!*******************************************************************
\headerfile   C_GameObject.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/29
\brief
              Header file for GameObject component class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <any>
#include "C_Base.h"

enum class ObjectType
{
    Door,
    Box,
    MovingTile,
    PlateTower,
	FragileWall,
    Last
};

/*!*******************************************************************
\class C_GameObject
\brief
       Has types of object and logics for each.
********************************************************************/
class C_GameObject : public C_Base
{
public:
    C_GameObject() : C_Base(Component::GameObject) {}

    /*!*******************************************************************
    \brief
           Overrided method from C_Base class that reads configuration
           file for this component.

    \param sstream
           stringstream that has information for this component.
    ********************************************************************/
    void ReadIn(std::stringstream & sstream) override;

    /*!*******************************************************************
    \brief
           Getter method for object type.

    \return ObjectType
            Type of object.
    ********************************************************************/
    ObjectType GetType() const { return m_type; }

    /*!*******************************************************************
    \brief
           Getter method for object.

    \return void*
            Pointer to object.
    ********************************************************************/
    std::any* Get_Object() { return &m_object; }

private:
    ObjectType m_type; //!< Type of object.
    std::any m_object; //!< Pointer to class of m_type of object.
};

#endif //C_GAMEOBJECT_H