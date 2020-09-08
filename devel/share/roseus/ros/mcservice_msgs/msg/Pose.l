;; Auto-generated. Do not edit!


(when (boundp 'mcservice_msgs::Pose)
  (if (not (find-package "MCSERVICE_MSGS"))
    (make-package "MCSERVICE_MSGS"))
  (shadow 'Pose (find-package "MCSERVICE_MSGS")))
(unless (find-package "MCSERVICE_MSGS::POSE")
  (make-package "MCSERVICE_MSGS::POSE"))

(in-package "ROS")
;;//! \htmlinclude Pose.msg.html


(defclass mcservice_msgs::Pose
  :super ros::object
  :slots (_latitude _longitude _altitude ))

(defmethod mcservice_msgs::Pose
  (:init
   (&key
    ((:latitude __latitude) 0.0)
    ((:longitude __longitude) 0.0)
    ((:altitude __altitude) 0.0)
    )
   (send-super :init)
   (setq _latitude (float __latitude))
   (setq _longitude (float __longitude))
   (setq _altitude (float __altitude))
   self)
  (:latitude
   (&optional __latitude)
   (if __latitude (setq _latitude __latitude)) _latitude)
  (:longitude
   (&optional __longitude)
   (if __longitude (setq _longitude __longitude)) _longitude)
  (:altitude
   (&optional __altitude)
   (if __altitude (setq _altitude __altitude)) _altitude)
  (:serialization-length
   ()
   (+
    ;; float64 _latitude
    8
    ;; float64 _longitude
    8
    ;; float64 _altitude
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _latitude
       (sys::poke _latitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _longitude
       (sys::poke _longitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _altitude
       (sys::poke _altitude (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _latitude
     (setq _latitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _longitude
     (setq _longitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _altitude
     (setq _altitude (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get mcservice_msgs::Pose :md5sum-) "c48027a852aeff972be80478ff38e81a")
(setf (get mcservice_msgs::Pose :datatype-) "mcservice_msgs/Pose")
(setf (get mcservice_msgs::Pose :definition-)
      "float64 latitude
float64 longitude
float64 altitude

")



(provide :mcservice_msgs/Pose "c48027a852aeff972be80478ff38e81a")


