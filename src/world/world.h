/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef WORLD_H
#define WORLD_H

#include <QMap>
#include <QReadWriteLock>
#include <proto/messages.pb.h>

/**
 * @brief The World class will allow the services to request and store upcoming data
 */
class World
{
public:
    World(); /*!< Default constructor */

    /**
     * Store the robots data in their specific color list
     *
     * @param robotList The list containing the robots
     */
    void setRobotsData(QList<Robot> robotList);

    /**
     * Store the robot data in their specific color list
     *
     * @param robot Robot class containing the data
     */
    void setRobotData(const Robot* robot);

    /**
     * Store the robot data in their specific color list
     *
     * @param robot Robot class containing the data
     */
    void setRobotStatus(const RobotStatus* robotStatus);

    /**
     * Store the robot data in their specific color list
     *
     * @param robot Robot class containing the data
     */
    void setRobotsStatus(QList<RobotStatus> robotStatusList);

    /**
     * Store the field data
     *
     * @param field Field class containing the data
     */
    void setFieldData(const Field* field);

    /**
     * Store the ball data
     *
     * @param ball Ball class containing the data
     */
    void setBallData(const Ball* ball);

    /**
     *
     * @param color The color which the returned robots are assigned
     * @return QList containing all the stored robots from the specified color
     */
    QList<Robot> getRobotsFromColor(const Color* color);

    /**
     *
     * @param identifier The robot identifier which the search will be made
     * @return Robot pointer containing the data from the searched robot
     */
    Robot* getRobotData(const RobotIdentifier* identifier);

    /**
     *
     * @return Field class containing the data
     */
    Field* getFieldData();

    /**
     *
     * @return Ball class containing the data
     */
    Ball* getBallData();

private:
    // Internal variables
    QMap<bool, QList<Robot>*> _robots; /*!< Stores, for each team color, an list of its robots */
    Field _field; /*!< Stores the latest Field data */
    Ball _ball; /*!< Stores the latest Ball data */

    // Mutexes
    QReadWriteLock _robotMutex; /*!< Responsible for Robot data */
    QReadWriteLock _fieldMutex; /*!< Responsible for Field data */
    QReadWriteLock _ballMutex; /*!< Responsible for Ball data */
};

#endif // WORLD_H
