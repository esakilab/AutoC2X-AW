;; Auto-generated. Do not edit!


(when (boundp 'mcservice_msgs::Trajectory)
  (if (not (find-package "MCSERVICE_MSGS"))
    (make-package "MCSERVICE_MSGS"))
  (shadow 'Trajectory (find-package "MCSERVICE_MSGS")))
(unless (find-package "MCSERVICE_MSGS::TRAJECTORY")
  (make-package "MCSERVICE_MSGS::TRAJECTORY"))

(in-package "ROS")
;;//! \htmlinclude Trajectory.msg.html


(defclass mcservice_msgs::Trajectory
  :super ros::object
  :slots (_trajectory ))

(defmethod mcservice_msgs::Trajectory
  (:init
   (&key
    ((:trajectory __trajectory) (let (r) (dotimes (i 0) (push (instance mcservice_msgs::TrajectoryPoint :init) r)) r))
    )
   (send-super :init)
   (setq _trajectory __trajectory)
   self)
  (:trajectory
   (&rest __trajectory)
   (if (keywordp (car __trajectory))
       (send* _trajectory __trajectory)
     (progn
       (if __trajectory (setq _trajectory (car __trajectory)))
       _trajectory)))
  (:serialization-length
   ()
   (+
    ;; mcservice_msgs/TrajectoryPoint[] _trajectory
    (apply #'+ (send-all _trajectory :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; mcservice_msgs/TrajectoryPoint[] _trajectory
     (write-long (length _trajectory) s)
     (dolist (elem _trajectory)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; mcservice_msgs/TrajectoryPoint[] _trajectory
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _trajectory (let (r) (dotimes (i n) (push (instance mcservice_msgs::TrajectoryPoint :init) r)) r))
     (dolist (elem- _trajectory)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get mcservice_msgs::Trajectory :md5sum-) "2f7bdc402ec8ae3956a89db5336a8eba")
(setf (get mcservice_msgs::Trajectory :datatype-) "mcservice_msgs/Trajectory")
(setf (get mcservice_msgs::Trajectory :definition-)
      "mcservice_msgs/TrajectoryPoint[] trajectory

================================================================================
MSG: mcservice_msgs/TrajectoryPoint
int64 time
mcservice_msgs/Pose pose

================================================================================
MSG: mcservice_msgs/Pose
float64 latitude
float64 longitude
float64 altitude

")



(provide :mcservice_msgs/Trajectory "2f7bdc402ec8ae3956a89db5336a8eba")


