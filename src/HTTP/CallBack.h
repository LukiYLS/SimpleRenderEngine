#pragma once
#include <memory>
#include <string>
namespace HTTP {

	class ProgressCallBack {
	public:
		/**
		* Creates a new ProgressCallback
		*/
		ProgressCallBack() :
			_canceled(false),
			_needsRetry(false) {

		}
		virtual ~ProgressCallBack() { }

		typedef std::shared_ptr<ProgressCallBack> ptr;

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
		virtual bool reportProgress(
			double             current,
			double             total,
			unsigned           currentStage,
			unsigned           totalStages,
			const std::string& msg) {
			return false;
		}

		/**
		* Convenience functions
		*/
		bool reportProgress(double current, double total, const std::string& msg) {
			return reportProgress(current, total, 0, 1, msg);
		}
		bool reportProgress(double current, double total) {
			return reportProgress(current, total, 0, 1, "");
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
		void setNeedsRetry(bool needsRetry) { _needsRetry = needsRetry; }

	protected:
		volatile unsigned _numStages;
		std::string       _message;
		mutable  bool     _needsRetry;
		mutable  bool     _canceled;
	};
}