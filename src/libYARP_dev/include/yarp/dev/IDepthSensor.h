/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Alberto Cardellino
 * email:   alberto.cardellino@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#ifndef YARP_DEV_IDEPTHSENSOR_H
#define YARP_DEV_IDEPTHSENSOR_H

#include <yarp/dev/DeviceDriver.h>
#include <yarp/sig/Matrix.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Stamp.h>

// Common usage vocabs (also defined in ControlBoardInterfaces.h)
#define VOCAB_SET                   VOCAB3('s','e','t')
#define VOCAB_GET                   VOCAB3('g','e','t')
#define VOCAB_IS                    VOCAB2('i','s')
#define VOCAB_OK                    VOCAB2('o','k')
#define VOCAB_SUCCESS               VOCAB4('s','u','c','c')
#define VOCAB_FAILED                VOCAB4('f','a','i','l')

#define VOCAB_IDEPTH_SENSOR         VOCAB4('i','d','p','t')
#define VOCAB_SENSOR_INFO           VOCAB4('i','n','f','o')
#define VOCAB_SENSOR_STATUS         VOCAB4('s','t','a','t')
#define VOCAB_DISTANCE_RANGE        VOCAB4('r','a','n','g')
#define VOCAB_HSCAN_LIMIT           VOCAB4('h','l','i','m')
#define VOCAB_VSCAN_LIMIT           VOCAB4('v','l','i','m')
#define VOCAB_DATA_SIZE             VOCAB4('s','i','z','e')
#define VOCAB_HV_RESOLUTION         VOCAB3('r','e','s')
#define VOCAB_SCAN_RATE             VOCAB4('r','a','t','e')
#define VOCAB_DEPTH_MIRROR          VOCAB4('d','m','i','r')

#define VOCAB_DEPTH_WIDTH           VOCAB4('d','w','i','d')
#define VOCAB_DEPTH_HEIGHT          VOCAB4('d','h','e','i')


namespace yarp {
    namespace dev {
        class IDepthSensor;
    }
}

/**
 * @ingroup dev_iface_other
 *
 * A generic interface for depth sensor device
 * This implementation is targeted for depth cameras like
 * kinect devices.
 * For linear scanner, the IRangefinder2D interface shall be used instead.
 *
 * This interface could be extended in the future to take
 * into account other devices like planar lasers or single point
 * range measurement devices like ultrasound systems, infrared proximity
 * sensors and so on, but not yet at this time.
 *
 * Laser or light devices can easily fit into this interface while
 * sonar devices doesn't, so a different interface/device shall be used instead.
 */

class YARP_dev_API yarp::dev::IDepthSensor
{
public:
    enum DepthSensor_status
    {
        DEPTHSENSOR_NOT_READY        = 0,
        DEPTHSENSOR_OK_STANBY        = 1,
        DEPTHSENSOR_OK_IN_USE        = 2,
        DEPTHSENSOR_GENERAL_ERROR    = 3,
        DEPTHSENSOR_TIMEOUT          = 4
    };

    virtual ~IDepthSensor(){}

   /**
    * get the device hardware charactestics
    * @param device_info Searchable struct containing the device info
    * @return true if able to get information about the device.
    */
    virtual bool getDeviceInfo(yarp::os::Searchable *device_info) = 0;

   /**
    * Get the distance measurements as an image
    * @param ranges the vector containing the distance measurement
    * @return true if able to get measurement data.
    */
    virtual bool getMeasurementData(yarp::sig::FlexImage &image, yarp::os::Stamp *stamp=NULL) = 0;

   /**
    * get the device status
    * @param status the device status
    * @return true/false.
    */
    virtual bool getDeviceStatus(DepthSensor_status *status) = 0;

   /**
    * get the device detection range
    * @param min the minimum detection distance from the sensor [meter]
    * @param max the maximum detection distance from the sensor [meter]
    * @return true if able to get required info.
    */
    virtual bool getDistanceRange(double *min, double *max) = 0;

   /**
    * set the device detection range. Invalid setting will be discarded.
    * @param min the minimum detection distance from the sensor [meter]
    * @param max the maximum detection distance from the sensor [meter]
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool setDistanceRange(double min, double max) = 0;

   /**
    * get the horizontal scan limits / field of view with respect to the
    * front line of sight of the sensor. Angles are measured around the
    * positive Z axis (counterclockwise, if Z is up) with zero angle being
    * forward along the x axis
    * @param min start angle of the scan  [degrees]
    * @param max end angle of the scan    [degrees]
    * @return true if able to get required info.
    */
    virtual bool getHorizontalScanLimits(double *min, double *max) = 0;

   /**
    * set the horizontal scan limits / field of view with respect to the
    * front line of sight of the sensor. Angles are measured around the
    * positive Z axis (counterclockwise, if Z is up) with zero angle being
    * forward along the x axis
    * @param min start angle of the scan  [degrees]
    * @param max end angle of the scan    [degrees]
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool setHorizontalScanLimits(double min, double max) = 0;

   /**
    * get the vertical scan limits / field of view with respect to the
    * front line of sight of the sensor   [degrees]
    * @param min start angle of the scan  [degrees]
    * @param max end angle of the scan    [degrees]
    * @return true if able to get required info.
    */
    virtual bool getVerticalScanLimits(double *min, double *max) = 0;

   /**
    * set the vertical scan limits / field of view with respect to the
    * front line of sight of the sensor   [degrees]
    * @param min start angle of the scan  [degrees]
    * @param max end angle of the scan    [degrees]
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool setVerticalScanLimits(double min, double max) = 0;

   /**
    * Get the height of data matrix measured by the device.
    * It can be height of image for camera-like devices, or any number of points for
    * other devices. For un-ordered data stream it shall be equal to 1.
    * @param height   height of image, number of points in the vertical scan [dimension-less]
    * @return true if able to get required info.
    */
    virtual int height() const = 0;

   /**
    * Get the width of data matrix measured by the device.
    * It can be width of image for camera-like devices, while
    * for un-ordered data stream it shall be equal to number of data sent.
    * @param width  width of image, number of points in the vertical scan [dimension-less]
    * @return true if message was correctly delivered to the HW device.
    */
    virtual int width() const = 0;

   /**
    * get the device resolution, using the current settings of scan limits
    * and data size. Will return the resolution of device at 1 meter distance.
    * @param hRes horizontal resolution [meter]
    * @param vRes vertical resolution [meter]
    * @return true if able to get required info.
    */
    virtual bool getResolution(double *hRes, double *vRes) = 0;

   /**
    * set the device resolution.
    * This call can change the current settings of scan limits, data size or scan rate
    * to match the requested resolution.
    * Verify those settings is suggested after this call.
    * Will set the resolution of device at 1meter distance, if possible.
    * @param hRes horizontal resolution [meter]
    * @param vRes vertical resolution [meter]
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool setResolution(double hRes, double vRes) = 0;

   /**
    * get the scan rate (scans per seconds)
    * @param rate the scan rate
    * @return true if able to get required info.
    */
    virtual bool getScanRate(double *rate) = 0;

   /**
    * set the scan rate (scans per seconds)
    * @param rate the scan rate
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool setScanRate(double rate) = 0;

   /**
    * Set the mirroring option of the HW device for the depth camera.
    * NOTE: this setting ONLY influence the depth image, if the device
    * has also rgb image, the mirroring setting for the rgb camera has to be
    * configured independently using proper interface.
    * If set to true, the image sent by the device will be mirrored on X axis
    * @param mirror the mirroring option
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool setDepthMirror(bool mirror) = 0;

   /**
    * Get the mirroring option of the HW device.
    * NOTE: this get ONLY return setting of the depth image, if the device
    * has also rgb image, the mirroring setting for the rgb camera has to be
    * read independently using proper interface.
    * @param mirror the mirroring setting returned by the call
    * @return true if message was correctly delivered to the HW device.
    */
    virtual bool getDepthMirror(bool *mirror) = 0;
};

#endif   // YARP_DEV_IDEPTHSENSOR_H
