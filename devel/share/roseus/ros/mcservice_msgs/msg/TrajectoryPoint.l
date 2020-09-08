;; Auto-generated. Do not edit!


(when (boundp 'mcservice_msgs::TrajectoryPoint)
  (if (not (find-package "MCSERVICE_MSGS"))
    (make-package "MCSERVICE_MSGS"))
  (shadow 'TrajectoryPoint (find-package "MCSERVICE_MSGS")))
(unless (find-package "MCSERVICE_MSGS::TRAJECTORYPOINT")
  (make-package "MCSERVICE_MSGS::TRAJECTORYPOINT"))

(in-package "ROS")
;;//! \htmlinclude TrajectoryPoint.msg.html


(defclass mcservice_msgs::TrajectoryPoint
  :super ros::object
  :slots (_time _pose ))

(defmethod mcservice_msgs::TrajectoryPoint
  (:init
   (&key
    ((:time __time) 0)
    ((:pose __pose) (instance mcservice_msgs::Pose :init))
    )
   (send-super :init)
   (setq _time (round __time))
   (setq _pose __pose)
   self)
  (:time
   (&optional __time)
   (if __time (setq _time __time)) _time)
  (:pose
   (&rest __pose)
   (if (keywordp (car __pose))
       (send* _pose __pose)
     (progn
       (if __pose (setq _pose (car __pose)))
       _pose)))
  (:serialization-length
   ()
   (+
    ;; int64 _time
    8
    ;; mcservice_msgs/Pose _pose
    (send _pose :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int64 _time
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _time (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _time) (= (length (_time . bv)) 2)) ;; bignum
              (write-long (ash (elt (_time . bv) 0) 0) s)
              (write-long (ash (elt (_time . bv) 1) -1) s))
             ((and (class _time) (= (length (_time . bv)) 1)) ;; big1
              (write-long (elt (_time . bv) 0) s)
              (write-long (if (>= _time 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _time s)(write-long (if (>= _time 0) 0 #xffffffff) s)))
     ;; mcservice_msgs/Pose _pose
       (send _pose :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int64 _time
#+(or :alpha :irix6 :x86_64)
      (setf _time (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _time (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; mcservice_msgs/Pose _pose
     (send _pose :deserialize buf ptr-) (incf ptr- (send _pose :serialization-length))
   ;;
   self)
  )

(setf (get mcservice_msgs::TrajectoryPoint :md5sum-) "479ff4a676bfd08768071dc36141e72e")
(setf (get mcservice_msgs::TrajectoryPoint :datatype-) "mcservice_msgs/TrajectoryPoint")
(setf (get mcservice_msgs::TrajectoryPoint :definition-)
      "int64 time
mcservice_msgs/Pose pose

================================================================================
MSG: mcservice_msgs/Pose
float64 latitude
float64 longitude
float64 altitude

")



(provide :mcservice_msgs/TrajectoryPoint "479ff4a676bfd08768071dc36141e72e")


