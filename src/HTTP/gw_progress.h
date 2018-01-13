#include <string>

#include "gw_shared_ptr.h"

#ifndef gw_progress_h__
#define gw_progress_h__

namespace gw_http
{
    /**
    * ProgressCallback is a general purpose interface for functions that need to report progress.
    */
    class progress_callback
    {
    public:
        /**
        * Creates a new ProgressCallback
        */
        progress_callback();
        virtual ~progress_callback() { }

		typedef gw_shared_ptr<progress_callback> ptr;

        /**
        * Callback function that will be called.
        * @param current
        *        The amount of work done in the current stage
        * @param total
        *        The total amount of work to be done in the current stage
        * @param stage
        *        Stage of the operation we're currently in
        * @param totalStages
        *        Total number of stages in the operation
        * @param msg
        *        Description of what is being done. Useful when total is unknown.
        * @param returns
        *        Returns true if the current task should be cancelled, false othegwise.
        */
        virtual bool report_progress(
            double             current, 
            double             total, 
            unsigned           currentStage,
            unsigned           totalStages,
            const std::string& msg );

        /**
         * Convenience functions
         */
        bool report_progress( double current, double total, const std::string& msg ) {
            return report_progress(current, total, 0, 1, msg );
        }
        bool report_progress( double current, double total ) {
            return report_progress(current, total, 0, 1, "" );
        }

        /**
         * called when the process starts
         */
        virtual void onStarted() { }

        /**
         * called when the process completed 
         */
        virtual void onCompleted() { }

        /**
         * Sets the cancelation flag
         */
        void cancel() { _canceled = true; }

        /**
         * Whether cancelation was requested
         */
        virtual bool isCanceled() const { return _canceled; }

        std::string& message() { return _message; }

        /**
        *Whether or not the task should be retried.
        */
        bool needsRetry() const { return _needsRetry; }

        /**
         * Sets whether or not the task should be retried
         */
        void setNeedsRetry( bool needsRetry ) { _needsRetry = needsRetry; }

    protected:
        volatile unsigned _numStages;
        std::string       _message;
        mutable  bool     _needsRetry;
        mutable  bool     _canceled;
    };


    /**
    * console_progress_callback is a simple ProgressCallback that reports progress to the console
    */
    class console_progress_callback 
		: public progress_callback
    {
    public:
        /**
        * Creates a new console_progress_callback
        */
        console_progress_callback();
        virtual ~console_progress_callback() { }

        /**
        * Callback function that will be called.
        * @param current
        *        The amount of work done in the current stage
        * @param total
        *        The total amount of work to be done in the current stage
        * @param stage
        *        Stage of the operation we're currently in
        * @param totalStages
        *        Total number of stages in the operation
        * @param msg
        *        Description of what is being done. Useful when total is unknown.
        * @param returns
        *        Returns true if the current task should be cancelled, false othegwise.
        */
        virtual bool report_progress(
            double             current, 
            double             total, 
            unsigned           currentStage,
            unsigned           totalStages,
            const std::string& msg );
    };
}

#endif // gw_progress_h__


