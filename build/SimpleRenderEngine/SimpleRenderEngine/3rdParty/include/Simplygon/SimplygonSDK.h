#ifndef __SimplygonSDK_h__
#define __SimplygonSDK_h__

#include <string.h>

#ifdef _WIN32 // Microsoft compiler
#if (_MSC_VER >= 1300) // 7.1
#define SGDEPRECATED __declspec(deprecated)
#else
#define SGDEPRECATED
#endif
#else
#define SGDEPRECATED
#endif

namespace SimplygonSDK
{

	/** 
	 * CountedPointer is a template class for pointers to API objects. All API interface classes have typedefs for counted pointers that points at the specific class. 
	 * All such typedefs are prefixed with 'sp'. An example of this is 'spObject', which is a typedef for CountedPointer<IObject>. The CountedPointer can be used
	 * for any object that supports reference counting, and exports the AddRef and Release methods. See the IObject class for an example.
	 */ 
	template <class T> class CountedPointer
		{
		public:
			/**
			 * Constructs a CounterPointer from a standard pointer. If the source pointer points at an API object, a reference to the API object is added.
			 * @param p is a pointer to an API object, or NULL to make the CountedPointer point at nothing.
			 */
			CountedPointer( T *p=NULL ) : ptr(p)
				{
				addref_ptr();
				}
				
			/**
			 * Constructs a CountedPointer from another CountedPointer. If the source pointer points at an API object, a reference to the API object is added.
			 * @param p is a CountedPointer that points at an API object.
			 */
			CountedPointer( const CountedPointer<T> &p ) : ptr(p.ptr)
				{
				addref_ptr();
				}
				
			/**
			 * Destructs the CountedPointer, and releases one reference to the API object, if the CountedPointer does currently point at an object.
			 */
			~CountedPointer()
				{
				release_ptr();
				}
		
			/** 
			 * Tells wether the CountedPointer points at an object, or nothing.
			 * @return true if the pointer points at nothing, of false if the pointer points at an object
			 */
			bool IsNull() const
				{
				return (ptr == NULL);
				}
				
			/**
			 * Sets the pointer to point at the same object as another CountedPointer
			 * @param p is the source CountedPointer object
			 * @return a referece to this object
			 */
			CountedPointer<T>& operator=( const CountedPointer<T> &p ) 
				{
				if( this != &p ) 
					{
					release_ptr();
					ptr = p.ptr;
					addref_ptr();
					}
				return *this;
				}

			/**
			 * Operator that returns the reference of the object the pointer is pointing at. Note! Do not use this operator if the pointer is not pointing at an object. 
			 * @return a referece to the object the pointer is pointing at
			 */
		    T& operator*() const 
				{
		        return *ptr;
				}
				
			/**
			 * Operator that returns a standard pointer to the object the pointer is pointing at. 
			 * @return a pointer to the object the pointer is pointing at
			 */
		    T* operator->() const 
				{
		        return ptr;
				}
								
			/**
			 * Operator that returns a standard pointer to the object the pointer is pointing at. 
			 * @return a pointer to the object the pointer is pointing at
			 */
			operator T* () const
				{
				return ptr;
				}
				
			/**
			 * Method that returns a standard pointer to the object the pointer is pointing at. 
			 * @return a pointer to the object the pointer is pointing at
			 */
			T* GetPointer() const
				{
				return ptr;
				}
				
		private:
			void addref_ptr()
				{
				if( ptr!=NULL )
					{
					ptr->AddRef();
					}
				}
		
			void release_ptr()
				{
				if( ptr!=NULL )
					{
					ptr->Release();
					ptr = NULL;
					}
				}
		
			T *ptr;	
	    };
		

	class IObject;
	
	/** 
	 * rhandle are used to represent opaque handles returned from and used by the API
	 */
	typedef void* rhandle;
	
	/** 
	 * rid represents ids within the API
	 */
	typedef int rid;

	/**
	 * real represents generic floating point values, depending on API floating point
	 * precision version, reals are either typedef:ed as floats or doubles
	 */
	typedef float real;
			
	const real REAL_MIN = 1.175494351e-38F;
	const real REAL_MAX = 3.402823466e+38F;

	/** 
	 * rchar represents text characters within the API. 
	 */
	typedef char rchar;

	/** 
	 * The robserver class is used to receive events from an interface, by creating a class that
	 * implements the Execute method. By adding an object of the class into an interface using 
	 * the AddObserver method, the Execute method will receive all events of the specified EventId
	 */
	class robserver
		{
		public:
			/** 
			 * The Execute method will receive all events that the robserver is registered to receive.
			 * @param subject is the object that is sending the event
			 * @param EventId is the id of the sent event
			 * @param EventParameterBlock points at a event-specific parameter data block
			 * @param EventParameterBlockSize is the size of the event-specific data block, 
			 * which is useful if the Execute method passes the information on to another handler. Please
			 * note that if the block data is copied to another handler, the data may have to be copied
			 * back into the original block to be able to pass data back to the API class. This is 
			 * however event-specific.
			 */
			virtual void Execute( IObject *subject , rid EventId , void *EventParameterBlock , unsigned int EventParameterBlockSize ) = 0;	
		};

	/**
	 * The rerrorhandler class is used to receive error messages from the interface, by by creating a class that
	 * implements the HandleError method. By setting an object of the class into the main interface using 
	 * the SetErrorHandler method, the HandleError method will receive all error messages from the interface.
	 * Please note that errors may not be recoverable, and could leave objects in an intermediate state. Errors
	 * should in most cases be considered fatal, and will not occur i standard use. 
	 */
	class rerrorhandler
		{
		public:
			/** 
			 * The HandleError method will receive all error messages from the interface.
			 * @param object is the interface object that raised the error message
			 * @param interfacename is the name of the interface 
			 * @param methodname is the name of the method in the object that was called when the error occured
			 * @param errortype is the type of error that has occured
			 * @param errortext is a description of the error that has occured
			 */
			virtual void HandleError( IObject *object , const char *interfacename , const char *methodname , rid errortype , const char *errortext ) = 0;
		};

	/**
	 * The IString class is used to represent strings that are created by the API objects, and returned to the caller.
	 * The strings are reference counted and constant strings, and can be retrieved, but not modified. All
	 * methods that input a string value does this using basic null-terminated rchar-pointers.
	 * For simplicity it is recommended to never use the IString interface directly, but instead through an 
	 * rstring object, which inherits from a CountedPointer of IString, and that also adds help code and conversions.
	 */
	class IString
		{
		protected:
			rchar *Text; // the text of the string (warning: never modify this pointer directly)

		public:
			virtual void AddRef() = 0;
			virtual void Release() = 0;
			virtual const rchar *GetText() const = 0;
			virtual bool IsEmpty() const = 0;
		};
	
	typedef CountedPointer<IString> spString;
	
	/**
	 * The rstring is a help class that handles type conversion and reference counting for the IString 
	 * interface.
	 */
	class rstring : public CountedPointer<IString>
		{
		public:
			rstring( IString *ptr = NULL ) : CountedPointer<IString>( ptr ) {}
			rstring( const CountedPointer<IString> &other ) : CountedPointer<IString>( other ) {}
			rstring( const rstring &other ) : CountedPointer<IString>( other ) {}
			
			/** 
			 * Operator that sets the rstring to point at the same IString as the source rstring
			 */
			rstring& operator=( const rstring &p ) 
				{
				CountedPointer<IString>::operator=( p );
				return *this;
				}

			/** 
			 * Method to retrieve the string value as a const rchar *. If the rstring does not point at an object, NULL is returned
			 */
			const rchar *GetText() const
				{
				IString *p = this->GetPointer();
				if( !p )
					{
					return NULL;
					}
				return p->GetText();
				}

			/** 
			 * Operator to cast the rstring into a const rchar *. If the rstring does not point at an object, NULL is returned
			 */
			operator const rchar *() const
				{
				return this->GetText();
				}
				
			/**
			 * Method to check if the rstring is empty, which is the case if the rstring does not point at an IString object,
			 * the IString object does not contain a string, or the string exists, but has zero length.
			 */
			bool IsEmpty() const 
				{
				IString *p = this->GetPointer();
				if( !p )
					{
					return true;
					}
				return p->IsEmpty();
				}
				
		};



	/** 
	 * EventCodes are used by the event system to specify what kind of event to listen for
	 */
	enum EventCodes
		{
		/// updates the current progress, in percent
		SG_EVENT_PROGRESS = 0x1000001, 

		/// tells observers when a new processing has started
		SG_EVENT_PROCESS_STARTED = 0x1000002, 
		};

	/** 
	 * Error codes that are returned on initialization/finalization of the library
	 */
	enum ErrorCodes 
		{
		SG_ERROR_NOERROR = 0,
		SG_ERROR_NOLICENSE = -1, // no license was found (or licence expired)
		SG_ERROR_NOTINITIALIZED = -2, // the SDK is not initialized, or no process object has been loaded/created
		SG_ERROR_ALREADYINITIALIZED = -3, // the SDK is already initialized
		SG_ERROR_FILENOTFOUND = -4, // the specified file was not found
		SG_ERROR_INVALIDPARAM = -5, // an invalid parameter was passed to the method
		SG_ERROR_FAILEDTESTING = -6, // the reduction failed post-testing
		SG_ERROR_WRONGVERSION = -7, // the Simplygon DLL and header file interface versions do not match
		SG_ERROR_LOADFAILED = -8, // the Simplygon DLL failed loading, probably because of a missing dependency
		};

	/** 
	 * GeometryValidationCodes are used by the Geometry Validation system to report 
	 * errors in a setup geometry.
	 */
	enum GeometryValidationCodes 
		{
		SG_VALIDATIONERROR_NOERROR = 0,
		SG_VALIDATIONERROR_ILLEGAL_TRIANGLECOUNT = -1,
		SG_VALIDATIONERROR_ILLEGAL_VERTEXCOUNT = -2,
		SG_VALIDATIONERROR_ILLEGAL_INDICES = -3,
		SG_VALIDATIONERROR_ILLEGAL_TRIANGLE = -4,
		SG_VALIDATIONERROR_ILLEGAL_FIELD_ITEMCOUNT = -5,
		SG_VALIDATIONERROR_ILLEGAL_FIELD_TUPLECOUNT = -6,
		SG_VALIDATIONERROR_ILLEGAL_FIELD_TUPLESIZE = -7
		};

	/** 
	 * FeatureFlags are used to mask what different features to consider when processing.
	 */
	enum FeatureFlags
		{
		SG_FEATUREFLAGS_GEOMETRYBOUNDARY	=	0x00000001,
		SG_FEATUREFLAGS_MATERIAL			=	0x00000002,
		SG_FEATUREFLAGS_SHADING				=	0x00000004,
		SG_FEATUREFLAGS_USER				=	0x00000008,
		SG_FEATUREFLAGS_GROUP				=	0x00000010,
		SG_FEATUREFLAGS_NONMANIFOLD			=	0x00000020, // contains edges connecting more than 2 triangles
		SG_FEATUREFLAGS_FLIPPEDTRIANGLE		=	0X00000040,
		SG_FEATUREFLAGS_NORMALCHART			=	0X00000080,
		SG_FEATUREFLAGS_COLORS				=	0x00000100,
		SG_FEATUREFLAGS_SYMMETRY			=	0x00000200,
		SG_FEATUREFLAGS_BORDEREND			=	0x00000400, // only contains border on 1 edge (2 half-edges) for example, end of a sharp-edge
		SG_FEATUREFLAGS_BORDERJUNCTION 		=	0x00000800, // contains atleast 3 border-edges
		SG_FEATUREFLAGS_COMPONENTCONNECTOR	=	0x00001000, // connects 2 or more triangle-groups that arent edge-connected
		SG_FEATUREFLAGS_NONCONNECTED   		=	0x00002000, // not used in any triangle
		SG_FEATUREFLAGS_TEXTURE0			=	0x00010000,
		SG_FEATUREFLAGS_TEXTURE1			=	0x00020000,
		SG_FEATUREFLAGS_TEXTURE2			=	0x00040000,
		SG_FEATUREFLAGS_TEXTURE3			=	0x00080000,
		SG_FEATUREFLAGS_TEXTURE4			=	0x00100000,
		SG_FEATUREFLAGS_TEXTURE5			=	0x00200000,
		SG_FEATUREFLAGS_TEXTURE6			=	0x00400000,
		SG_FEATUREFLAGS_TEXTURE7			=	0x00800000,
		SG_FEATUREFLAGS_TEXTURE_ALL			=	0x00ff0000,
		SG_FEATUREFLAGS_ALL					=	0xffffffff,
		};

	/** 
	 * StopCondition tells if reducer will stop when trianglecount and/or deviation has been reached
	 */
	enum StopCondition
		{
		SG_STOPCONDITION_REDUCTIONRATIO		=	0,
		SG_STOPCONDITION_MAXDEVIATION		=	1,
		SG_STOPCONDITION_BOTH_ARE_REACHED	=	2,
		SG_STOPCONDITION_EITHER_IS_REACHED	=	3,		
		};

	/** 
	 * BoneProcessing tells if the number of bones that the vertices are linked to should be reduced or not, 
	   and in what way that should happen. It can either calculate with the help of pixelsize how many bones
	   should be removed, or a set percentage (ratio, 0.0 - 1.0 * the original bonescount) of the bones can be removed.

	   It is albo possible to not touch the number of bones used, but only reduce the number of bones that each vertex can be linked to.
	 */

	enum BoneRemovalProcessing {
		SG_BONEPROCESSING_NO_PROCESSING		=	0,
		SG_BONEPROCESSING_NO_BONE_REMOVAL	=	1,
		SG_BONEPROCESSING_RATIO_PROCESSING	=	2,
		SG_BONEPROCESSING_PIXELSIZE_PROCESSING	=	3,
		};
		
	/** 
	 * TangentSpaceMethod is used to specify which tangent space method is used
	 * to generate tangent spaces when processing. Use SG_TANGENTSPACEMETHOD_ORTHONORMAL_LEFTHANDED
	 * to generate tangents that are D3D compatible (left-handed coordinate system).
	 */
	enum TangentSpaceMethod
		{
		/// Standard method, creates an orthonormal tangent space (right-handed)
		SG_TANGENTSPACEMETHOD_ORTHONORMAL = 0,
		
		/// 3dsMax compatible method, creates tangent space that is compatible with the 3dsMax renderer
		SG_TANGENTSPACEMETHOD_3DSMAX = 1,
		
		/// Creates an orthonormal tangent space (left-handed, D3D compatible)
		SG_TANGENTSPACEMETHOD_ORTHONORMAL_LEFTHANDED = 2,
		};


	/**
	* TextureBlendType specifies which type of blend function to use to blend textures.
	* The blend type function will be used while casting channels during material casting.
	*/
	enum TextureBlendType {
		SG_TEXTUREBLEND_REPLACE	=	0,
		SG_TEXTUREBLEND_ADD		=	1,
		SG_TEXTUREBLEND_SUBTRACT	=	2,
		SG_TEXTUREBLEND_MULTIPLY	=	3,
		SG_TEXTUREBLEND_ALPHA	=	4,
		SG_TEXTUREBLEND_PRE_MULTIPLIED_ALPHA	=	5,
		};

	/** 
	 * The SG_MATERIAL_CHANNEL_[...] defines the default names of 
	 * standard material textures in Simplygon.
	 */
	static const char *SG_MATERIAL_CHANNEL_AMBIENT = "Ambient";
	static const char *SG_MATERIAL_CHANNEL_DIFFUSE = "Diffuse";
	static const char *SG_MATERIAL_CHANNEL_SPECULAR = "Specular";
	static const char *SG_MATERIAL_CHANNEL_OPACITY = "Opacity";
	static const char *SG_MATERIAL_CHANNEL_EMISSIVE = "Emissive";
	static const char *SG_MATERIAL_CHANNEL_NORMALS = "Normals";
	static const char *SG_MATERIAL_CHANNEL_GROUPIDS = "GroupIds";
	static const char *SG_MATERIAL_CHANNEL_DISPLACEMENT = "Displacement";
	
	
	/** 
	 * The SG_IMAGEDATA_FORMAT defines the default names of 
	 * standard image formats in Simplygon.
	 */
	static const char *SG_IMAGEDATA_FORMAT_L = "L";
	static const char *SG_IMAGEDATA_FORMAT_RGB = "RGB";
	static const char *SG_IMAGEDATA_FORMAT_RGBA = "RGBA";
	
	/**
	 * The number of supported texture and color channels in Simplygon
	 */
	static const unsigned int SG_NUM_SUPPORTED_TEXTURE_CHANNELS = 256;
	static const unsigned int SG_NUM_SUPPORTED_COLOR_CHANNELS = 256;

	/**
	 * The maximum number of supported bones per vertex in Simplygon
	 */
	static const unsigned int SG_NUM_SUPPORTED_BONES_PER_VERTEX = 16;


	/**
	 * The number of supported mapping layers in Simplygon
	 */
	static const unsigned int SG_NUM_SUPPORTED_MAPPING_LAYERS = 32;

	/**
	 * \deprecated SG_BONES_PER_VERTEX is deprecated, use SG_NUM_SUPPORTED_BONES_PER_VERTEX instead
	 */
	static const unsigned int SG_BONES_PER_VERTEX = SG_NUM_SUPPORTED_BONES_PER_VERTEX;

	/** 
	 * \deprecated Deprecated values, use SG_MATERIAL_CHANNEL_[...] instead
	 */
	static const char *SG_MATERIAL_TEXTURE_AMBIENT = SG_MATERIAL_CHANNEL_AMBIENT;
	static const char *SG_MATERIAL_TEXTURE_DIFFUSE = SG_MATERIAL_CHANNEL_DIFFUSE;
	static const char *SG_MATERIAL_TEXTURE_SPECULAR = SG_MATERIAL_CHANNEL_SPECULAR;
	static const char *SG_MATERIAL_TEXTURE_OPACITY = SG_MATERIAL_CHANNEL_OPACITY;
	static const char *SG_MATERIAL_TEXTURE_EMISSIVE = SG_MATERIAL_CHANNEL_EMISSIVE;
	static const char *SG_MATERIAL_TEXTURE_NORMALS = SG_MATERIAL_CHANNEL_NORMALS;
	static const char *SG_MATERIAL_TEXTURE_GROUPIDS = SG_MATERIAL_CHANNEL_GROUPIDS;
	static const char *SG_MATERIAL_TEXTURE_DISPLACEMENT = SG_MATERIAL_CHANNEL_DISPLACEMENT;

	#ifdef _WIN32 
	#pragma deprecated( SG_BONES_PER_VERTEX )
	#pragma deprecated( SG_MATERIAL_TEXTURE_AMBIENT , SG_MATERIAL_TEXTURE_DIFFUSE , SG_MATERIAL_TEXTURE_SPECULAR )
	#pragma deprecated( SG_MATERIAL_TEXTURE_OPACITY , SG_MATERIAL_TEXTURE_EMISSIVE , SG_MATERIAL_TEXTURE_NORMALS )
	#pragma deprecated( SG_MATERIAL_TEXTURE_GROUPIDS , SG_MATERIAL_TEXTURE_DISPLACEMENT )
	#endif



	/** 
	 * BaseTypes are ids of the base types used by data containers (arrays etc.) within Simplygon.
	 */
	enum BaseTypes
		{
		TYPES_ID_BOOL = 1,
		TYPES_ID_CHAR = 2,
		TYPES_ID_UCHAR = 3,
		TYPES_ID_SHORT = 4,
		TYPES_ID_USHORT = 5,
		TYPES_ID_INT = 6,
		TYPES_ID_UINT = 7,
		TYPES_ID_LONG = 8,
		TYPES_ID_ULONG = 9,		
		TYPES_ID_FLOAT = 10,
		TYPES_ID_DOUBLE = 11,
		TYPES_ID_REAL = 12,
		TYPES_ID_RID = 13,
		TYPES_ID_RCHAR = 14,
		TYPES_ID_RHANDLE = 15,
		TYPES_ID_OBJECT	= 16
		};

	
	/**
	 * IObject is the base class for all normal classes defined within the API. The class adds safe casting into any API object, reference counting support
	 * (best used with the CountedPointer reference counted pointer class), basic names for all objects, and event handling, using observers (using objects
	 * derived from robserver)
	 */
	class IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();
		
		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;
		
		/**
		 * The IsClassA function returns true if the class is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( type && (strcmp("IObject",type)==0) )
				return true;
			return false;
			}
			
		/** 
		* SafeCast makes sure the pointer is of a class that can be cast into IObject, and if this is the case, returns the pointer cast into an IObject 
		* @param ptr is the pointer to be cast into a pointer to an object of type IObject
		* @return a pointer to IObject, if the cast can be made
		*/
		static IObject *SafeCast( IObject *ptr ) { return ptr; }

		/**
		 * PrintInfo prints the contents/info of the object to the log
		 */
		virtual void PrintInfo() = 0;

		/**
		 * Adds one reference to the object.
		 */
		virtual void AddRef() = 0;

		/**
		 * Releases one reference to the object. When all references have been removed, the object is removed from memory.
		 */
		virtual void Release() = 0;
		
		/** 
		 * Sets the name of the object.
		 * @param Name is the new name of the object
		 */
		virtual void SetName( const rchar *Name ) = 0;
		
		/** 
		 * Gets the name of the object.
		 * @return The name of the object
		 */
		virtual rstring GetName() = 0;
	
		/** 
		 * Adds a used-defined observer object to the interface, that will receive any event with the specified EventId from the interface
		 * @param Observer is the object that will receive events
		 * @param EventId the event id to listen for
		 * @return the id of the observer within the API object
		 */
		virtual rid AddObserver( robserver *Observer , rid EventId ) = 0;
		
		/** 
		 * Removes a previously added observer object
		 * @param ObserverId the id returned by AddObserver when the observer was added
		 */
		virtual void RemoveObserver( rid ObserverId ) = 0;

		virtual void *GetUserComponentArea( rid userid ) = 0;
	};

	/**
	 * a CounterPointer smart pointer to an IObject
	 */
	typedef CountedPointer<IObject> spObject;
	class IArray;
	class IRidArray;

	/**
	 * ReArray is the base class for the array objects. The  object contains a dynamically
	 * allocated list of tuples of the same  length, which can be specified before the list
	 * is filled with values.
	 */
	class IArray : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IArray",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IArray pointer
		 * @return a pointer to the IArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IArray") )
				return static_cast<IArray*>(ptr);
			return NULL;
			}
		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize.
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlaying data types.
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly on component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array. 
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * GetBaseType returns the id of the basic type that the array is based on The ids are
		 * listed in the BaseTypes enumeration
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Appends an array to this array. Note that both arrays must be of  the same classtype
		 * and have the same tuple size. (If both arrays are  descendants of ReValueArray, they
		 * can be of different types, but the  conversion is slow).
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Creates a new array with the same underlaying data type as this array. To also copy
		 * the actual data, set copy_data to true.
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IArray
	 */
	typedef CountedPointer<IArray> spArray;

	class IArray;
	class IRidArray;
	class IValueArray;
	class IRealArray;

	/**
	 * ReValueArray adds methods to arrays to generically  add, set and get tuples, through
	 * real values.
	 */
	class IValueArray : public IArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IValueArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IValueArray",type)==0 )
				return true;
			return IArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IValueArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IValueArray pointer
		 * @return a pointer to the IValueArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IValueArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IValueArray") )
				return static_cast<IValueArray*>(ptr);
			return NULL;
			}
		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Appends an array to this array. Note that both arrays must be of  the same classtype
		 * and have the same tuple size. (If both arrays are  descendants of ReValueArray, they
		 * can be of different types, but the  conversion is slow).
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize.
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to. GetRealItem converts the value from  the actual underlying
		 * data type to real.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Creates a new array with the same underlaying data type as this array. To also copy
		 * the actual data, set copy_data to true.
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to. SetRealItem converts the value from real to the actual
		 * underlying data type.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly on component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlaying data types.
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array. 
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * GetBaseType returns the id of the basic type that the array is based on The ids are
		 * listed in the BaseTypes enumeration
		 */
		virtual	rid GetBaseType(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IValueArray
	 */
	typedef CountedPointer<IValueArray> spValueArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReStringArray is the string implementation of ReValueArray. See ReValueArray for
	 * a description. Please note that conversions to/from real values will not work on
	 * string arrays.
	 */
	class IStringArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IStringArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IStringArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IStringArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IStringArray pointer
		 * @return a pointer to the IStringArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IStringArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IStringArray") )
				return static_cast<IStringArray*>(ptr);
			return NULL;
			}
		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	rstring GetItem( rid id ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( const char * value ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , const char * value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IStringArray
	 */
	typedef CountedPointer<IStringArray> spStringArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReBoolArray is the bool implementation of ReValueArray. See ReValueArray for a description.
	 */
	class IBoolArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IBoolArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IBoolArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IBoolArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IBoolArray pointer
		 * @return a pointer to the IBoolArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IBoolArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IBoolArray") )
				return static_cast<IBoolArray*>(ptr);
			return NULL;
			}
		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( bool *tuple ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , bool *tuple ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , bool value ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	bool GetItem( rid id ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , bool *dest_param ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( bool value ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( bool *dest_param ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( bool *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , bool *dest_param ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , bool *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IBoolArray
	 */
	typedef CountedPointer<IBoolArray> spBoolArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReCharArray is the char implementation of ReValueArray. See ReValueArray for a description.
	 */
	class ICharArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ICharArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ICharArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ICharArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ICharArray pointer
		 * @return a pointer to the ICharArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ICharArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ICharArray") )
				return static_cast<ICharArray*>(ptr);
			return NULL;
			}
		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , char *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( char *tuple ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( char *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , char *dest_param ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , char value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( char *dest_param ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	char GetItem( rid id ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , char *tuple ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( char value ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , char *dest_param ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ICharArray
	 */
	typedef CountedPointer<ICharArray> spCharArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReDoubleArray is the double implementation of ReValueArray. See ReValueArray for
	 * a description.
	 */
	class IDoubleArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IDoubleArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IDoubleArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IDoubleArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IDoubleArray pointer
		 * @return a pointer to the IDoubleArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IDoubleArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IDoubleArray") )
				return static_cast<IDoubleArray*>(ptr);
			return NULL;
			}
		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	double GetItem( rid id ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , double *dest_param ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , double *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( double value ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , double value ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( double *tuple ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , double *tuple ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( double *dest_param ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , double *dest_param ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( double *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IDoubleArray
	 */
	typedef CountedPointer<IDoubleArray> spDoubleArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReFloatArray is the float implementation of ReValueArray. See ReValueArray for a
	 * description.
	 */
	class IFloatArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IFloatArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IFloatArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IFloatArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IFloatArray pointer
		 * @return a pointer to the IFloatArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IFloatArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IFloatArray") )
				return static_cast<IFloatArray*>(ptr);
			return NULL;
			}
		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , float value ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	float GetItem( rid id ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( float *tuple ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , float *tuple ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , float *dest_param ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , float *dest_param ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( float value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( float *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , float *source_items , unsigned int source_item_count ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( float *dest_param ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IFloatArray
	 */
	typedef CountedPointer<IFloatArray> spFloatArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReIntArray is the int implementation of ReValueArray. See ReValueArray for a description.
	 */
	class IIntArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IIntArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IIntArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IIntArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IIntArray pointer
		 * @return a pointer to the IIntArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IIntArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IIntArray") )
				return static_cast<IIntArray*>(ptr);
			return NULL;
			}
		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , int *source_items , unsigned int source_item_count ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( int *dest_param ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( int value ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( int *tuple ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , int *dest_param ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( int *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	int GetItem( rid id ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , int *dest_param ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , int value ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , int *tuple ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IIntArray
	 */
	typedef CountedPointer<IIntArray> spIntArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReLongArray is the long implementation of ReValueArray. See ReValueArray for a description.
	 */
	class ILongArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ILongArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ILongArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ILongArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ILongArray pointer
		 * @return a pointer to the ILongArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ILongArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ILongArray") )
				return static_cast<ILongArray*>(ptr);
			return NULL;
			}
		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( long value ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , long *dest_param ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , long *tuple ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( long *source_items , unsigned int source_item_count ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , long *source_items , unsigned int source_item_count ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , long value ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , long *dest_param ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( long *dest_param ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	long GetItem( rid id ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( long *tuple ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ILongArray
	 */
	typedef CountedPointer<ILongArray> spLongArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReRcharArray is the rchar implementation of ReValueArray. See ReValueArray for a
	 * description.
	 */
	class IRcharArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRcharArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRcharArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRcharArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRcharArray pointer
		 * @return a pointer to the IRcharArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRcharArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRcharArray") )
				return static_cast<IRcharArray*>(ptr);
			return NULL;
			}
		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRcharArray
	 */
	typedef CountedPointer<IRcharArray> spRcharArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReRealArray is the real implementation of ReValueArray. See ReValueArray for a description.
	 */
	class IRealArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRealArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRealArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRealArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRealArray pointer
		 * @return a pointer to the IRealArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRealArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRealArray") )
				return static_cast<IRealArray*>(ptr);
			return NULL;
			}
		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( real *source_items , unsigned int source_item_count ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , real *dest_param ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , real *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	real GetItem( rid id ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , real value ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( real value ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( real *tuple ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , real *tuple ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , real *dest_param ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( real *dest_param ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRealArray
	 */
	typedef CountedPointer<IRealArray> spRealArray;

	class IArray;
	class IRidArray;

	/**
	 * ReRhandleArray is the rhandle implementation of ReArray. See ReArray for a description.
	 */
	class IRhandleArray : public IArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRhandleArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRhandleArray",type)==0 )
				return true;
			return IArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRhandleArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRhandleArray pointer
		 * @return a pointer to the IRhandleArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRhandleArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRhandleArray") )
				return static_cast<IRhandleArray*>(ptr);
			return NULL;
			}
		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetTuple to set the items directly.
		 */
		virtual	void AddTuple( rhandle *tuple ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly on component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each.
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( rhandle *dest_param ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , rhandle *dest_param ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode.
		 */
		virtual	void SetItem( rid id , rhandle value ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( rhandle *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode
		 */
		virtual	void GetTuple( rid id , rhandle *dest_param ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array.
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , rhandle *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetTuple to set the items directly.
		 */
		virtual	void AddItem( rhandle value ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same underlying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array. 
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Appends an array to this array. The array must be of the  same type as this one.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Returns the id of the last item that is in the array. Warning! Do not confuse this
		 * id with the id of a tuple, they are only the same if TupleSize is equal to one, i.e.
		 * when the tuples are exactly on component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode.
		 */
		virtual	void SetTuple( rid id , rhandle *tuple ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Creates a new array with the same underlying data type as this array. To also copy
		 * the actual data, set copy_data to true.
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode.
		 */
		virtual	rhandle GetItem( rid id ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRhandleArray
	 */
	typedef CountedPointer<IRhandleArray> spRhandleArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReRidArray is the rid implementation of ReValueArray. See ReValueArray for a description.
	 */
	class IRidArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRidArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRidArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRidArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRidArray pointer
		 * @return a pointer to the IRidArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRidArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRidArray") )
				return static_cast<IRidArray*>(ptr);
			return NULL;
			}
		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , rid *tuple ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , rid *dest_param ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	rid GetItem( rid id ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( rid value ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , rid value ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( rid *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( rid *tuple ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( rid *dest_param ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , rid *dest_param ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , rid *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRidArray
	 */
	typedef CountedPointer<IRidArray> spRidArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReShortArray is the short implementation of ReValueArray. See ReValueArray for a
	 * description.
	 */
	class IShortArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IShortArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IShortArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IShortArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IShortArray pointer
		 * @return a pointer to the IShortArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IShortArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IShortArray") )
				return static_cast<IShortArray*>(ptr);
			return NULL;
			}
		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( short *tuple ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , short *dest_param ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( short *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( short value ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , short *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , short *dest_param ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , short value ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , short *tuple ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( short *dest_param ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	short GetItem( rid id ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IShortArray
	 */
	typedef CountedPointer<IShortArray> spShortArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReUnsignedCharArray is the uchar implementation of ReValueArray. See ReValueArray
	 * for a description.
	 */
	class IUnsignedCharArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IUnsignedCharArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IUnsignedCharArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IUnsignedCharArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IUnsignedCharArray pointer
		 * @return a pointer to the IUnsignedCharArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IUnsignedCharArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IUnsignedCharArray") )
				return static_cast<IUnsignedCharArray*>(ptr);
			return NULL;
			}
		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , unsigned char *dest_param ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( unsigned char *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , unsigned char value ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( unsigned char value ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , unsigned char *source_items , unsigned int source_item_count ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( unsigned char *tuple ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	unsigned char GetItem( rid id ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , unsigned char *tuple ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , unsigned char *dest_param ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( unsigned char *dest_param ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IUnsignedCharArray
	 */
	typedef CountedPointer<IUnsignedCharArray> spUnsignedCharArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReUnsignedIntArray is the uint implementation of ReValueArray. See ReValueArray for
	 * a description.
	 */
	class IUnsignedIntArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IUnsignedIntArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IUnsignedIntArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IUnsignedIntArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IUnsignedIntArray pointer
		 * @return a pointer to the IUnsignedIntArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IUnsignedIntArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IUnsignedIntArray") )
				return static_cast<IUnsignedIntArray*>(ptr);
			return NULL;
			}
		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( unsigned int *tuple ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( unsigned int value ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( unsigned int *dest_param ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , unsigned int *dest_param ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( unsigned int *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , unsigned int *tuple ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , unsigned int *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , unsigned int *dest_param ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	unsigned int GetItem( rid id ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , unsigned int value ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IUnsignedIntArray
	 */
	typedef CountedPointer<IUnsignedIntArray> spUnsignedIntArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReUnsignedLongArray is the ulong implementation of ReValueArray. See ReValueArray
	 * for a description.
	 */
	class IUnsignedLongArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IUnsignedLongArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IUnsignedLongArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IUnsignedLongArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IUnsignedLongArray pointer
		 * @return a pointer to the IUnsignedLongArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IUnsignedLongArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IUnsignedLongArray") )
				return static_cast<IUnsignedLongArray*>(ptr);
			return NULL;
			}
		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , unsigned long *tuple ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , unsigned long *dest_param ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , unsigned long value ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( unsigned long *dest_param ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , unsigned long *dest_param ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( unsigned long *tuple ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( unsigned long value ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	unsigned long GetItem( rid id ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( unsigned long *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , unsigned long *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IUnsignedLongArray
	 */
	typedef CountedPointer<IUnsignedLongArray> spUnsignedLongArray;

	class IValueArray;
	class IRidArray;
	class IRealArray;
	class IArray;

	/**
	 * ReUnsignedShortArray is the ushort implementation of ReValueArray. See ReValueArray
	 * for a description.
	 */
	class IUnsignedShortArray : public IValueArray
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IUnsignedShortArray is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IUnsignedShortArray",type)==0 )
				return true;
			return IValueArray::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IUnsignedShortArray pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IUnsignedShortArray pointer
		 * @return a pointer to the IUnsignedShortArray object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IUnsignedShortArray *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IUnsignedShortArray") )
				return static_cast<IUnsignedShortArray*>(ptr);
			return NULL;
			}
		/**
		 * CopyRange copyies a range of tuples from a source array into this  array. The arrays
		 * must have the same tuplesize and base type. This array must be resized to hold the
		 * tuples before copying. @param source_array the source array to copy from @param start_dest_id
		 * the first destination tuple id @param start_src_id the first source tuple id @param
		 * count the number of tuples to copy
		 */
		virtual	void CopyRange( IArray *source_array , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Sets a tuple in the list. The id is the id of the tuple that is being updated. Warning!
		 * No range checking is done in  release mode. @param id - the id of the tuple  @param
		 * tuple - the new value(s) of the tuple 
		 */
		virtual	void SetTuple( rid id , unsigned short *tuple ) = 0;

		/**
		 * Returns a range of tuples in the array.
		 */
		virtual	void GetDataRange( rid start_tuple_id , unsigned int tuple_count , unsigned short *dest_param ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CopyCobineTuples, but  there is three source tuples, and two
		 * alpha values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1
		 * + alpha_2) dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 The source array
		 * must be of the same type as this array, and have the same number of items in each
		 * tuple. This array can be used as the source array to copy within the array.
		 */
		virtual	void CopyCombine3Tuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Copies tuples from a source array through an id table. The  id table dictates the
		 * order in which the tuples are to be copied to  this array. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source array. The id table is assumed to have a tuple
		 * size of 1. The source array and this array must have the same underlaying data type,
		 * and have the same tuple size. startId is the first tuple that will receive the cpoied
		 * values The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCopy( IArray *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	void SetInterpolateTuples( bool value ) = 0;

		/**
		 * Returns the number of items in the array Warning! Do not confuse this count with
		 * the count of tuples, they are only the same if TupleSize is equal to one, i.e.  when
		 * the tuples are exactly one component each.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Clears the array.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	rstring GetAlternativeName(  ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	void SetTupleSize( unsigned int newsize ) = 0;

		/**
		 * Copies data from another array. The arrays must have the same underlying data types.
		 * @param source the source array
		 */
		virtual	void DeepCopy( IArray *source ) = 0;

		/**
		 * Extracts tuples from the array. Note that the destination array must have the same
		 * classtype and have the same tuple size as the source array. (If both arrays are decendants
		 * of ReValueArray, they can be of  different types, but the conversion is slow).
		 */
		virtual	void ExtractTuples( IArray *dest , rid start , unsigned int count ) = 0;

		/**
		 * Get/Set the AlternativeName, which can be used by users to set an alternative name
		 * to the array, when used as field data or in a collection. 
		 */
		virtual	void SetAlternativeName( const char * value ) = 0;

		/**
		 * Creates a new array with the same undelaying data type as this array. To also copy
		 * the actual data, set copy_data to true. @return the new copy of the array
		 */
		virtual	CountedPointer<IArray> NewCopy( bool copy_data ) = 0;

		/**
		 * SetRealItem retrieves the item at position id.  The id is the id of the item, not
		 * the tuple the item belongs to.
		 */
		virtual	real GetRealItem( rid id ) = 0;

		/**
		 * Adds an item to the list, allocates memory as needed to enlarge the list. If many
		 * items is being put into the list, use Resize to set the size of the new list (only
		 * one reallocation needed) and then use SetItem / SetRealTuple to set the items directly.
		 * @param value - the value of the item
		 */
		virtual	void AddItem( unsigned short value ) = 0;

		/**
		 * Returns the id of the last item that is in the array. The id returned is equal to
		 * (ItemCount - 1).  Warning! Do not confuse this id with the id of a tuple, they are
		 * only the same if TupleSize is equal to one, i.e.  when the tuples are exactly one
		 * component each.
		 */
		virtual	rid GetMaxItemId(  ) = 0;

		/**
		 * Sets an item in the list. The id of the item is the exact location of the item (TupleId
		 * * TupleSize + Index of item in tuple) Warning! No range checking is done in release
		 * mode. @param id - the id of the item  @param value - the new value of the item
		 */
		virtual	void SetItem( rid id , unsigned short value ) = 0;

		/**
		 * Used to get/set the tuple size. Warning! Only use SetTupleSize before filling the
		 * list with values, as any  added data in the array will be trashed.
		 */
		virtual	unsigned int GetTupleSize(  ) = 0;

		/**
		 * CompareTuples compares a tuple in this array with a tuple in another array
		 */
		virtual	int CompareTuples( IArray *otherArray , rid tuple_id , rid other_tuple_id ) = 0;

		/**
		 * Adds a tuple to the list. Reallocates the list if needed. If many items is being
		 * put into the list, use Resize to set the  size of the new list (only one reallocation
		 * needed) and then use  SetItem / SetRealTuple to set the items directly. @param tuple
		 * - the value(s) of the tuple 
		 */
		virtual	void AddTuple( unsigned short *tuple ) = 0;

		/**
		 * Gets the value of an item in the list. The id of the item  is the exact location
		 * of the item (TupleId * TupleSize + Index  of item in tuple) Warning! No range checking
		 * is done in release mode. @param id - the id of the item 
		 */
		virtual	unsigned short GetItem( rid id ) = 0;

		/**
		 * Set/Get the interpolation flag. If set, the tuple values will be  interpolated by
		 * calls to CobineTuples and BarycentricCombineTuples. If not set, then CobineTuples
		 * and BarycentricCombineTuples will copy data from the tuple with the highest alpha
		 * blend value.
		 */
		virtual	bool GetInterpolateTuples(  ) = 0;

		/**
		 * CopyTuple copies one tuple in a source array to a tuple in this array. Both the dest_id
		 * and the source_id must exist in the arrays.  This array can be used as the source
		 * array to copy within the array. The arrays must be of the same type.
		 */
		virtual	void CopyTuple( IArray *sourceArray , rid dest_id , rid src_id ) = 0;

		/**
		 * Resizes the list of items to the exact count specified. Warning! Do not confuse this
		 * count with the count of tuples, they are only the same if TupleSize is equal to one,
		 * i.e.  when the tuples are exactly on component each. Also note that  the item count
		 * should be a multiple of TupleSize, as  TupleCount = ItemCount / TupleSize. @param
		 * count - the new number of items
		 */
		virtual	void SetItemCount( unsigned int count ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data.
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Set the array from a range of data values. Note that the array will be resized to
		 * the size of the items. Note that the source_item_count is the number of items to
		 * set, NOT the number of tuples.
		 */
		virtual	void SetData( unsigned short *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Appends an array to this array. The array must be a decendant of ReValueArray. For
		 * best performance, append arrays of the same basic type.
		 */
		virtual	void AppendTuples( IArray *source ) = 0;

		/**
		 * Returns all tuples in the array as a single item array.
		 */
		virtual	void GetData( unsigned short *dest_param ) = 0;

		/**
		 * Set a part of the array from a range of data values. Note that the array will be
		 * resized to contain the size of the items. Note that the source_item_count is the
		 * number of items to set, NOT the number of tuples, but start_tuple_id IS the first
		 * tuple index to update.
		 */
		virtual	void SetDataRange( rid start_tuple_id , unsigned short *source_items , unsigned int source_item_count ) = 0;

		/**
		 * Gets a tuple in the list. The id is the id of the tuple that is being fetched. Warning!
		 * No range checking is done in  release mode @param id - the id of the tuple  @param
		 * dest_param the destination tuple
		 */
		virtual	void GetTuple( rid id , unsigned short *dest_param ) = 0;

		/**
		 * SetRealItem sets the item at position id.  The id is the id of the item, not the
		 * tuple the item belongs to.
		 */
		virtual	void SetRealItem( rid id , real value ) = 0;

		/**
		 * GetBaseType returns the id of the type that the array is based on
		 */
		virtual	rid GetBaseType(  ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt bleand values. startId is the first tuple that will receive
		 * the combined values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Returns true if the list is empty.
		 */
		virtual	bool IsEmpty(  ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values. startId is the first tuple that will receive the combined
		 * values Note! The array must be large enough to hold the copied data.
		 */
		virtual	void IndexedCombine3( IValueArray *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * CopyCobineTuples combines the data from two tuples from a source array into a destination
		 * tuple in this array. If InterpolateTuples id  set to true, dest_id will receive a
		 * linear interpolation of the values,  using the alpha value. If InterpolateTuples
		 * is set to false, then if alpha is less than 0.5 the contents of src_id_1 will end
		 * up in dest, else the  contents of src_id_2 will be copied. dest = src_1*(1-alpha)
		 * + src_2*alpha The source array must be of the same type as this array, and have the
		 * same number of items in each tuple. This array can be used as the source array to
		 * copy within the array.
		 */
		virtual	void CopyCobineTuples( IValueArray *sourceArray , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Returns the number of tuples in the list.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Returns the id of the tuple last in the list. If the array is empty, the value is
		 * unspecified Note: GetMaxTupleId() always returns (GetTupleCount()-1) as a signed
		 * id
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates an array with the same undelying data and tuple
		 * settings as the source array.  However, NewPackedCopy only copies unique tuples,
		 * and if the index_array parameter is set, this index array will contain the same number
		 * of tuples as the source array, and  with the ids of a specific tuples within the
		 * new copy of the array.  @param index_array the optional array that will receive the
		 * indices of the remapped tuples @return the new packed array
		 */
		virtual	CountedPointer<IArray> NewPackedCopy( IRidArray *index_array ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IUnsignedShortArray
	 */
	typedef CountedPointer<IUnsignedShortArray> spUnsignedShortArray;

	class IFieldData;
	class IRidArray;
	class IRealArray;
	class IValueArray;

	/**
	 * ReFieldData represents multiple fields of data. Each  field is implemented as an
	 * ReValueArray object, that has a unique  name, and can have complex components (such
	 * as scalars, vectors,  quaternions, tensors or matrices).  All fields are assumed
	 * to be of the same length. ReFieldData can be  seen as an array with complex tuples,
	 * where the component can be of  different types.
	 */
	class IFieldData : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IFieldData is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IFieldData",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IFieldData pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IFieldData pointer
		 * @return a pointer to the IFieldData object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IFieldData *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IFieldData") )
				return static_cast<IFieldData*>(ptr);
			return NULL;
			}
		/**
		 * CopyRange copyies a range of tuples from a source field object into this  field object.
		 * The field objects must have the same field setup. This field object must be resized
		 * to hold the tuples before copying. @param source the source field object to copy
		 * from @param start_dest_id the first destination tuple id @param start_src_id the
		 * first source tuple id @param count the number of tuples to copy
		 */
		virtual	void CopyRange( IFieldData *source , rid start_dest_id , rid start_src_id , unsigned int count ) = 0;

		/**
		 * Copies tuples from a source field data object through an  id table. The id table
		 * dictates the order in which the tuples are  to be copied. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source field data object. The id table is assumed
		 * to have a tuple  size of 1. The source and this field data object must have the 
		 * same underlaying data fields, of the same type and the same tuple sizes. The FieldData
		 * must have enough tuples to hold the new data.
		 */
		virtual	void IndexedCopy( IFieldData *source , IRidArray *idtable , rid startId ) = 0;

		/**
		 * Returns the next handle, or NULL if no more fields exist in the object.
		 */
		virtual	rhandle GetNextFieldHandle( rhandle hand ) = 0;

		/**
		 * Returnes the handle of the first field. If no  fields are added to the object, NULL
		 * is returned.
		 */
		virtual	rhandle GetFirstFieldHandle(  ) = 0;

		/**
		 * Appends another field data object to this object. The fields may be setup differently,
		 * but this will come at a significant performance penalty. If fields in the other field
		 * data object is missing in this object, these will be added, and the items in the
		 * old tuples will be set to 0.  @param other the field data object that is appended
		 * @param add_missing_fields if set to true, missing fields will be added to this field
		 * data object.
		 */
		virtual	void AppendTuples( IFieldData *other , bool add_missing_fields ) = 0;

		/**
		 * Returns the number of fields in the field data object. The ids of the fields range
		 * from 0 through (GetFieldCount()-1)
		 */
		virtual	unsigned int GetFieldCount(  ) = 0;

		/**
		 * Returns the field associated with the specified handle.
		 */
		virtual	CountedPointer<IValueArray> GetFieldWithHandle( rhandle hand ) = 0;

		/**
		 * Adds a field based on a base type. Only ReValueArray  objects are allowed. The created
		 * and added object is returned
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeField( rid base_type , unsigned int tuple_size , const char * name ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values.  Does the blend: dest = src1*blend1 + src2*blend2 +
		 * src3*(1-(blend1+blend2))  The FieldData must have enough tuples to hold the new data.
		 */
		virtual	void IndexedCombine3( IFieldData *source , rid *idtable , real *blendtable , unsigned int idtable_cnt , rid startId ) = 0;

		/**
		 * CopyCombine3Tuples combines the data from three tuples into a destination tuple.
		 * The call works like CobineTuples, but  there is three source tuples, and two alpha
		 * values. the destination will be weighted by the values: alpha_3 = 1-(alpha_1 + alpha_2)
		 * dest = src_1*alpha_1 + src_2*alpha_2 + src_3*alpha_3 This field data object can be
		 * used as the source field data object  to copy within the object.
		 */
		virtual	void CopyCombine3Tuples( IFieldData *source , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Returns the number of tuples.
		 */
		virtual	unsigned int GetTupleCount(  ) = 0;

		/**
		 * Compares the field setup of this field data object to another  field data object.
		 * If the setups are not identical, false is returned.  Note! IsSetupIdenticalTo will
		 * return false even if the same fields  exist in both field data objects, but are not
		 * in the same order. 
		 */
		virtual	bool IsSetupIdenticalTo( IFieldData *other ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data. AddTupleCount adds the tuplecount to the current tuplecount
		 */
		virtual	void SetTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Clears all fields from the object. Releases all data in the object.
		 */
		virtual	void RemoveAllFields(  ) = 0;

		/**
		 * Resizes the list to the specified tuplecount. If the list is enlarged, the new tuples
		 * will contain unspecified data. AddTupleCount adds the tuplecount to the current tuplecount
		 */
		virtual	void AddTupleCount( unsigned int tuplecount ) = 0;

		/**
		 * Creates another field data object with the same field  setup. To also copy the data
		 * to the new object, set copy_data to true.
		 */
		virtual	CountedPointer<IFieldData> NewCopy( bool copy_data ) = 0;

		/**
		 * Copies the field setup and data from another object. To only copy the setup, set
		 * copy_data to false.
		 */
		virtual	void DeepCopy( IFieldData *source , bool copy_data ) = 0;

		/**
		 * Returns the id of the field with the highest id. NOTE! If no fields exist in the
		 * field data object, the return is undefined.
		 */
		virtual	rid GetMaxFieldId(  ) = 0;

		/**
		 * Returns the id of the last tuple in the array. If the array is empty, the value is
		 * undefined.
		 */
		virtual	rid GetMaxTupleId(  ) = 0;

		/**
		 * Like NewCopy, NewPackedCopy creates a field data object with the same underlying
		 * data and tuple settings as the source field data object.  However, NewPackedCopy
		 * only copies unique tuples, and if the index_array parameter is set, this index array
		 * will contain the same number of tuples as the source  field data object, and with
		 * the ids of a specific tuples within the new copy of the  field data object. 
		 */
		virtual	CountedPointer<IFieldData> NewPackedCopy( IRidArray *index_array ) = 0;

		/**
		 * Retrieves a field. If the field was not found, the return is NULL.
		 */
		virtual	CountedPointer<IValueArray> GetField( const char * name ) = 0;

		/**
		 * CopyTuple copies one tuple to another. Both the dest_id and the source_id must exist
		 * in the array. This field data object can be used as the source field data object
		 * to copy within the object.
		 */
		virtual	void CopyTuple( IFieldData *source , rid dest_id , rid src_id ) = 0;

		/**
		 * CombineTuples combines the data from two tuples into a  destination tuple. This field
		 * data object can be used as the source  field data object to copy within the object.
		 */
		virtual	void CopyCombineTuples( IFieldData *source , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Clears all tuples from the fields. The fields,  however, are not removed.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Copies tuples from a source field data object through an  id table. The id table
		 * dictates the order in which the tuples are  to be copied. E.g. If the first item
		 * in the id table has the value 14, then the tuple with id 14 in the source array will
		 * be copied  to the first tuple in this array.  Note! All ids in the id table must
		 * be valid ids of tuples in the  source field data object. The id table is assumed
		 * to have a tuple  size of 1. The source and this field data object must have the 
		 * same underlaying data fields, of the same type and the same tuple sizes. The FieldData
		 * must have enough tuples to hold the new data.
		 */
		virtual	void IndexedCopy( IFieldData *source , rid *idtable , unsigned int idtable_cnt , rid startId ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt blend values.  Does the blend: dest = src1*(1-blend) + src2*blend
		 * The FieldData must have enough tuples to hold the new data.
		 */
		virtual	void IndexedCombine( IFieldData *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Works like IndexedCopy, but uses two consecutive ids in the idtable, and a blendvalue
		 * from the blendtable. The idtable contains (idtable_cnt*2) indices and blendtable
		 * contains idtable_cnt blend values.  Does the blend: dest = src1*(1-blend) + src2*blend
		 * The FieldData must have enough tuples to hold the new data.
		 */
		virtual	void IndexedCombine( IFieldData *source , rid *idtable , real *blendtable , unsigned int idtable_cnt , rid startId ) = 0;

		/**
		 * Works like IndexedCombine, but with three ids that are combined through two blendvalues
		 * in the blendtable. The idtable contains (idtable_cnt*3) indices and blendtable contains
		 * (idtable_cnt*2) blend values.  Does the blend: dest = src1*blend1 + src2*blend2 +
		 * src3*(1-(blend1+blend2))  The FieldData must have enough tuples to hold the new data.
		 */
		virtual	void IndexedCombine3( IFieldData *source , IRidArray *idtable , IRealArray *blendtable , rid startId ) = 0;

		/**
		 * Retrieves a field from its id.
		 */
		virtual	CountedPointer<IValueArray> GetFieldWithId( rid id ) = 0;

		/**
		 * Returns true if no tuples exist in the field data.
		 */
		virtual	int IsEmpty(  ) = 0;

		/**
		 * Removes a field. Removing a field causes the remaining fields to be remapped to new
		 * ids.
		 */
		virtual	void RemoveField( const char * name ) = 0;

		/**
		 * Extracts a range of tuples from this object. The recieveing object is assumed to
		 * have exactly the same data fields as this object. start is the id of the first  tuple
		 * that is extracted, and count is the number of tuples to extract.
		 */
		virtual	void ExtractTuples( IFieldData *dest , rid start , unsigned int count ) = 0;

		/**
		 * Adds a field. The field must have a unique name set.
		 */
		virtual	void AddField( IValueArray *field ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IFieldData
	 */
	typedef CountedPointer<IFieldData> spFieldData;

	class IGeometryData;
	class IPackedGeometryData;
	class IRealArray;
	class IBoolArray;
	class IRidArray;
	class IValueArray;
	class IFieldData;
	class IMatrix4x4;

	/**
	 * ReGeometryData represents a geometric structure  consisting of pointdata (Vertices)
	 * and topological data (Triangles). ReGeometryData may represent a whole geometric
	 * object, but can also be used by streaming data filters, and in this way, only represents
	 * a part of the object.  Different fields in the Vertices and Triangles fields data
	 * objects will contain pointdata and topological data. The standard  naming convension
	 * used in the filers/renderers are as follows: (Case sensitive naming, other field
	 * may also be present)  'Coords'	Positional coordinates of a vertex, expressed with
	 * a 3-component real field (XYZ).  'TexCoords0' - 'TexCoords15'	Texture coordinates
	 * of a vertex, expressed with a 2 components real field. By convention, the existing
	 * fields must be sequential, and  must start with 'TexCoord0'.  'Normals'	Normal vector
	 * for the vertex,	expressed  with a 3-component real field, a  normalized (XYZ) vector.
	 * 'VertexIds'	The id of the primitive's current vertex.  This field may be present
	 * even if the  vertex data	is directly specified in the  primitive's data, to specify
	 * topology. The field is of type rid.  'MaterialIds'	The material of the primitive.
	 * The field is of type rid.  The 'Coords' and 'VertexIds' fields always exist in the
	 * object, but the other fields are optional. Also, there can exist user fields.
	 */
	class IGeometryData : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGeometryData is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGeometryData",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGeometryData pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGeometryData pointer
		 * @return a pointer to the IGeometryData object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGeometryData *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGeometryData") )
				return static_cast<IGeometryData*>(ptr);
			return NULL;
			}
		/**
		 * Retrieves the ReFieldData object that contains the corner fields
		 */
		virtual	CountedPointer<IFieldData> GetCorners(  ) = 0;

		/**
		 * Adds/Removes/Gets a custom field. To be able to add a field, it must have a name
		 * that does not  conflict with the existing custom fields.
		 */
		virtual	void RemoveCustomField( const char * name ) = 0;

		/**
		 * Gets/Sets the number of vertices in the geometry. AddVertices increases the vertex
		 * count by the specified value.
		 */
		virtual	void AddVertices( unsigned int count ) = 0;

		/**
		 * Extracts specified triangles from the geometry. The destination geometry will contain
		 * the triangles and vertices.  The vertices in the destination will be expanded so
		 * that there exists one vertex  for each of the three corners of each triangle in the
		 * geometry.
		 */
		virtual	void ExtractTriangles( IGeometryData *dest , IRidArray *triangle_ids ) = 0;

		/**
		 * Adds/Removes/Gets a Color field in the corners.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'Colors0' - 'Colors15' (Field name: "Colors0"
		 * - "Colors15", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetColors( rid level ) = 0;

		/**
		 * Gets/Sets the number of triangles in the geometry. The number of corners is always
		 * set to (TriangleCount*3), so there is no specific method to set the number of corners.
		 * AddTriangles increases the triangle count by the specified value.
		 */
		virtual	unsigned int GetTriangleCount(  ) = 0;

		/**
		 * \deprecated GetDiffuseColors() is deprecated, use GetColors() instead.DiffuseColors
		 * is now an alias for 'Colors0'
		 */
		virtual	SGDEPRECATED CountedPointer<IRealArray> GetDiffuseColors(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	CountedPointer<IValueArray> GetUserVertexField( const char * name ) = 0;

		/**
		 * Adds/Removes/Gets the GroupIds field in the triangles. (Field name: "GroupIds", Tuple
		 * size: 1 , Stored as: triangle attribute)
		 */
		virtual	CountedPointer<IRidArray> GetGroupIds(  ) = 0;

		/**
		 * Retrieves the ReFieldData object that contains the vertex fields
		 */
		virtual	CountedPointer<IFieldData> GetVertices(  ) = 0;

		/**
		 * Adds/Removes/Gets a custom field. To be able to add a field, it must have a name
		 * that does not  conflict with the existing custom fields.
		 */
		virtual	CountedPointer<IValueArray> GetCustomField( const char * name ) = 0;

		/**
		 * Adds/Removes/Gets the MaterialIds field in the triangles. (Field name: "MaterialIds",
		 * Tuple size: 1 , Stored as: triangle attribute)
		 */
		virtual	void RemoveMaterialIds(  ) = 0;

		/**
		 * Gets/Sets the number of triangles in the geometry. The number of corners is always
		 * set to (TriangleCount*3), so there is no specific method to set the number of corners.
		 * AddTriangles increases the triangle count by the specified value.
		 */
		virtual	void AddTriangles( unsigned int count ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	void RemoveUserVertexField( const char * name ) = 0;

		/**
		 * Gets/Sets the number of triangles in the geometry. The number of corners is always
		 * set to (TriangleCount*3), so there is no specific method to set the number of corners.
		 * AddTriangles increases the triangle count by the specified value.
		 */
		virtual	void SetTriangleCount( unsigned int count ) = 0;

		/**
		 * Detect triangle edge neighbors. Neighbor ids are stored in the "EdgeNeighbours" Corner
		 * field.
		 */
		virtual	void DetectEdgeNeighbours(  ) = 0;

		/**
		 * \deprecated RemoveUserTriangleVertexField() is deprecated, use RemoveUserCornerField()
		 * instead.AddUserTriangleVertexField() is renamed AddUserCornerField() RemoveUserTriangleVertexField()
		 * is renamed RemoveUserCornerField() GetUserTriangleVertexField is renamed GetUserCornerField()
		 */
		virtual	SGDEPRECATED void RemoveUserTriangleVertexField( const char * name ) = 0;

		/**
		 * Copies all fields in the specified triangle in the source geometry to this geometry's
		 * triangle. CopyCombineTriangles combines two triangles by linear interopolation into
		 * the destination triangle. CopyCombine3Triangles combines three triangles via barycentric
		 * coordinates.
		 */
		virtual	void CopyTriangle( IGeometryData *source , rid dest_id , rid src_id ) = 0;

		/**
		 * Gets the Coords field in the vertices. (Field name: "Coords", Tuple size: 3 , Stored
		 * as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetCoords(  ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetBoneWeights(  ) = 0;

		/**
		 * Adds/Removes/Gets the Normals field in the corners.  (Field name: "Normals" , Tuple
		 * size: 3 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetNormals(  ) = 0;

		/**
		 * \deprecated AddBoneIds() is deprecated, use AddBoneWeights() instead.Function is
		 * empty, does now nothing. BoneIds are added/removed when AddBoneWeights()/RemoveBoneWeights()
		 * is called.
		 */
		virtual	SGDEPRECATED void AddBoneIds( unsigned int tuplesize ) = 0;

		/**
		 * Adds/Removes/Gets a custom field. To be able to add a field, it must have a name
		 * that does not  conflict with the existing custom fields.
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeCustomField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Calculate the extents by checking the coordinates of all vertices in the geometry.
		 * Set only_triangles to true to test only vertices that  are indexed by a triangle.
		 * This is a slower test, and  the bounding box may not encompass all vertices' positions.
		 */
		virtual	void CalculateExtents( bool only_triangles ) = 0;

		/**
		 * Adds/Removes/Gets the Normals field in the corners.  (Field name: "Normals" , Tuple
		 * size: 3 , Stored as: Corner attribute)
		 */
		virtual	void RemoveNormals(  ) = 0;

		/**
		 * Adds/Removes/Gets the VertexLocks field for the vertices. If the value for a vertex
		 * is true, then the vertex should be locked,  and not be removed in the collapse-process.
		 * (Field name: "VertexLocks", Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void AddVertexLocks(  ) = 0;

		/**
		 * Welds all vertices in the geometry that are closer than the welding threshold. Non-referenced
		 * vertices will not be removed, only the vertex indices will be remapped. Call Compact()
		 * to clean up  the geometry and free up memory.
		 */
		virtual	void Weld( real threshold ) = 0;

		/**
		 * Adds/Removes/Gets the GroupIds field in the triangles. (Field name: "GroupIds", Tuple
		 * size: 1 , Stored as: triangle attribute)
		 */
		virtual	void AddGroupIds(  ) = 0;

		/**
		 * Copies all fields in the specified triangle in the source geometry to this geometry's
		 * triangle. CopyCombineTriangles combines two triangles by linear interopolation into
		 * the destination triangle. CopyCombine3Triangles combines three triangles via barycentric
		 * coordinates.
		 */
		virtual	void CopyCombineTriangles( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Adds/Removes/Gets the VertexLocks field for the vertices. If the value for a vertex
		 * is true, then the vertex should be locked,  and not be removed in the collapse-process.
		 * (Field name: "VertexLocks", Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveVertexLocks(  ) = 0;

		/**
		 * \deprecated RemoveBoneIds() is deprecated, use RemoveBoneWeights() instead.Function
		 * is empty, does now nothing. BoneIds are added/removed when AddBoneWeights()/RemoveBoneWeights()
		 * is called.
		 */
		virtual	SGDEPRECATED void RemoveBoneIds(  ) = 0;

		/**
		 * Adds/Removes/Gets a Color field in the corners.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'Colors0' - 'Colors15' (Field name: "Colors0"
		 * - "Colors15", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	void AddColors( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified corner field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing corner fields.
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeUserCornerField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Adds/Removes/Gets the GroupIds field in the triangles. (Field name: "GroupIds", Tuple
		 * size: 1 , Stored as: triangle attribute)
		 */
		virtual	void RemoveGroupIds(  ) = 0;

		/**
		 * \deprecated AddUserTriangleVertexField() is deprecated, use AddUserCornerField()
		 * instead.AddUserTriangleVertexField() is renamed AddUserCornerField() RemoveUserTriangleVertexField()
		 * is renamed RemoveUserCornerField() GetUserTriangleVertexField is renamed GetUserCornerField()
		 */
		virtual	SGDEPRECATED void AddUserTriangleVertexField( IValueArray *field ) = 0;

		/**
		 * \deprecated GetSpecularColors() is deprecated, use GetColors() instead.SpecularColors
		 * is now an alias for 'Colors1'
		 */
		virtual	SGDEPRECATED CountedPointer<IRealArray> GetSpecularColors(  ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	CountedPointer<IRidArray> GetBoneIds(  ) = 0;

		/**
		 * Set/Get the inferior (minimum) extent of the geometry
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

		/**
		 * Adds/Removes/Gets the VertexLocks field for the vertices. If the value for a vertex
		 * is true, then the vertex should be locked,  and not be removed in the collapse-process.
		 * (Field name: "VertexLocks", Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IBoolArray> GetVertexLocks(  ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	void AddBoneWeights( unsigned int tuplesize ) = 0;

		/**
		 * Appends the geometry data to this geometry. This geometry will contain all triangles
		 * and vertices of both geometries. The  VertexIds that are copied from the source are
		 * remapped to point at the vertices, which are appended to the end of the current vertices.
		 * Any field in the appended geometry that does not exist in this geometry will be added,
		 * with the tuples that are added set to 0. The method will fail if the source geometry
		 * has identically named fields, but with different tuple sizes or base types. @param
		 * source the source geometry that will be appended to this geometry. the contents of
		 * source will not be modified.
		 */
		virtual	void AppendGeometry( IGeometryData *source ) = 0;

		/**
		 * Adds/Removes/Gets the MaterialIds field in the triangles. (Field name: "MaterialIds",
		 * Tuple size: 1 , Stored as: triangle attribute)
		 */
		virtual	void AddMaterialIds(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	void AddUserTriangleField( IValueArray *field ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified corner field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing corner fields.
		 */
		virtual	CountedPointer<IValueArray> GetUserCornerField( const char * name ) = 0;

		/**
		 * \deprecated RemoveDiffuseColors() is deprecated, use RemoveColors() instead.DiffuseColors
		 * is now an alias for 'Colors0'
		 */
		virtual	SGDEPRECATED void RemoveDiffuseColors(  ) = 0;

		/**
		 * Copies all fields in the specified triangle in the source geometry to this geometry's
		 * triangle. CopyCombineTriangles combines two triangles by linear interopolation into
		 * the destination triangle. CopyCombine3Triangles combines three triangles via barycentric
		 * coordinates.
		 */
		virtual	void CopyCombine3Triangles( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Adds/Removes/Gets a TexCoords field in the corners.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'TexCoords0' - 'TexCoords15' (Field name:
		 * "TexCoords0" - "TexCoords15", Tuple size: 2 , Stored as: Corner attribute)
		 */
		virtual	void AddTexCoords( rid level ) = 0;

		/**
		 * Gets the VertexIds field in the triangles.
		 */
		virtual	CountedPointer<IRidArray> GetVertexIds(  ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	void RemoveBoneWeights(  ) = 0;

		/**
		 * Tests if the extents of the geometry fully contain all vertex coordinates. Note!
		 * ExtentsContainCoords checks all vertices in the geometry, regardless to whether the
		 * vertex is referenced by any triangle. Use Compact() to remove any non-referenced
		 * vertex before  calling ExtentsContainCoords.
		 */
		virtual	bool ExtentsContainCoords(  ) = 0;

		/**
		 * Creates a "packed" RePackedGeometryData object, where all fields in the Corner field
		 * data object is moved to  the vertex field data object. Please note that the vertex
		 * field data object will be enlarged to accommodate for data in a vertex  shared by
		 * mutiple triangles, where the corner data differs between the triangles. The method
		 * will fail if a Corner field is named the same as an existing Vertices field, please
		 * make sure to remove any such field before calling the method.
		 */
		virtual	CountedPointer<IPackedGeometryData> NewPackedCopy(  ) = 0;

		/**
		 * Compacts the geometry, and removes any vertex tuple that  is not referenced by any
		 * triangle. If any tuple is removed, the  triangles will be remapped to the new vertex
		 * field.
		 */
		virtual	void Compact(  ) = 0;

		/**
		 * Adds/Removes/Gets the VertexWeighting field for the vertices. (Field name: "VertexWeighting",
		 * Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void AddVertexWeighting(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	void RemoveUserTriangleField( const char * name ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	void AddTangents( rid level ) = 0;

		/**
		 * Set/Get the inferior (minimum) extent of the geometry
		 */
		virtual	void SetInf( real vec[3] ) = 0;

		/**
		 * Adds/Removes/Gets the VertexWeighting field for the vertices. (Field name: "VertexWeighting",
		 * Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveVertexWeighting(  ) = 0;

		/**
		 * Adds/Removes/Gets a TexCoords field in the corners.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'TexCoords0' - 'TexCoords15' (Field name:
		 * "TexCoords0" - "TexCoords15", Tuple size: 2 , Stored as: Corner attribute)
		 */
		virtual	void RemoveTexCoords( rid level ) = 0;

		/**
		 * Adds/Removes/Gets the VertexWeighting field for the vertices. (Field name: "VertexWeighting",
		 * Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetVertexWeighting(  ) = 0;

		/**
		 * Copies the field setup and data from another object. To only copy the setup, set
		 * copy_data to false.
		 */
		virtual	void DeepCopy( IGeometryData *source , bool copy_data ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	void AddUserVertexField( IValueArray *field ) = 0;

		/**
		 * Gets/Sets the number of vertices in the geometry. AddVertices increases the vertex
		 * count by the specified value.
		 */
		virtual	void SetVertexCount( unsigned int count ) = 0;

		/**
		 * \deprecated CopyTriangleVertex() is deprecated, use CopyCorner() instead.CopyTriangleVertex()
		 * is renamed CopyCorner() CopyCombineTriangleVertices() is renamed CopyCombineCorners()
		 * CopyCombine3TriangleVertices() is renamed CopyCombine3Corners()
		 */
		virtual	SGDEPRECATED void CopyTriangleVertex( IGeometryData *source , rid dest_id , rid src_id ) = 0;

		/**
		 * Gets/Sets the number of vertices in the geometry. AddVertices increases the vertex
		 * count by the specified value.
		 */
		virtual	unsigned int GetVertexCount(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeUserVertexField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Set/Get the superior (maximum) extent of the geometry
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Copies all fields in the specified vertex in the source geometry to this geometry's
		 * vertex. CopyCombineVertices combines two vertices by linear interpolation into the
		 * destination vertex. CopyCombine3Vertices combines three vertices via barycentric
		 * coordinates.
		 */
		virtual	void CopyVertex( IGeometryData *source , rid dest_id , rid src_id ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	CountedPointer<IValueArray> GetUserTriangleField( const char * name ) = 0;

		/**
		 * Expands the Vertices field data so that there exists one vertex  for each of the
		 * three corners of each triangle in the geometry. The vertices will be organized identical
		 * to the Corners field. The triangles will be remapped to use the new vertices correctly.
		 * Expanding the vertex field is required to be able to extract a range of the geometry,
		 * and to be able to stream it.  Note! This will lead to increased memory requirement,
		 * as vertex  data is not shared among neigbouring triangles. It also removes the connectivity
		 * information of the triangles, and effectively  detaches all triangles from one another.
		 * This call may replace  fields in the Vertices field data. Requery any previously
		 * cached  field pointer after the call. All user fields are copied as well.
		 */
		virtual	void ExpandVertices(  ) = 0;

		/**
		 * Adds/Removes/Gets a TexCoords field in the corners.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'TexCoords0' - 'TexCoords15' (Field name:
		 * "TexCoords0" - "TexCoords15", Tuple size: 2 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetTexCoords( rid level ) = 0;

		/**
		 * Copies all fields in the specified vertex in the source geometry to this geometry's
		 * vertex. CopyCombineVertices combines two vertices by linear interpolation into the
		 * destination vertex. CopyCombine3Vertices combines three vertices via barycentric
		 * coordinates.
		 */
		virtual	void CopyCombineVertices( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * Copies all fields in the specified vertex in the source geometry to this geometry's
		 * vertex. CopyCombineVertices combines two vertices by linear interpolation into the
		 * destination vertex. CopyCombine3Vertices combines three vertices via barycentric
		 * coordinates.
		 */
		virtual	void CopyCombine3Vertices( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	void RemoveTangents( rid level ) = 0;

		/**
		 * Set/Get the superior (maximum) extent of the geometry
		 */
		virtual	void SetSup( real vec[3] ) = 0;

		/**
		 * \deprecated AddBaseTypeUserTriangleVertexField() is deprecated, use AddBaseTypeUserCornerField()
		 * instead.AddBaseTypeUserTriangleVertexField() has been renamed AddBaseTypeUserCornerField()
		 */
		virtual	SGDEPRECATED CountedPointer<IValueArray> AddBaseTypeUserTriangleVertexField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified corner field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing corner fields.
		 */
		virtual	void RemoveUserCornerField( const char * name ) = 0;

		/**
		 * Extracts a range of triangles with their vertices  from the geometry. ExpandVertices
		 * must first be called for the extraction to succeed. ExtractGeometry can be used to
		 * stream geometries in  chunks. If writing to a stream, the extracted geometry is guaranteed
		 * to have the same setup as the original. Any previous data in the destination will
		 * be deleted.
		 */
		virtual	void ExtractRange( IGeometryData *dest , rid start_tri_id , unsigned int count ) = 0;

		/**
		 * \deprecated RemoveSpecularColors() is deprecated, use RemoveColors() instead.SpecularColors
		 * is now an alias for 'Colors1'
		 */
		virtual	SGDEPRECATED void RemoveSpecularColors(  ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetTangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a Color field in the corners.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'Colors0' - 'Colors15' (Field name: "Colors0"
		 * - "Colors15", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	void RemoveColors( rid level ) = 0;

		/**
		 * Copies all fields in the specified corner in the source geometry to this geometrys'
		 * corner. CopyCombineCorner combines two triangles by linear interpolation into the
		 * destination triangle. CopyCombine3Corner combines three triangles via barycentric
		 * coordinates.
		 */
		virtual	void CopyCombine3Corners( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeUserTriangleField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Copies all fields in the specified corner in the source geometry to this geometrys'
		 * corner. CopyCombineCorner combines two triangles by linear interpolation into the
		 * destination triangle. CopyCombine3Corner combines three triangles via barycentric
		 * coordinates.
		 */
		virtual	void CopyCorner( IGeometryData *source , rid dest_id , rid src_id ) = 0;

		/**
		 * Transforms the geometry using the supplied matrix transformation. The fields that
		 * are modified are the Coords field of Vertices, as well as the Normals and all Tangent
		 * and Bitangent fields of the Corners
		 */
		virtual	void Transform( IMatrix4x4 *transformation ) = 0;

		/**
		 * \deprecated AddBitangents() is deprecated, use AddTangents() instead.Bitangents are
		 * Added and Removed in unison with Tangents. Only need  to call AddTangents to add
		 * both Tangent and Bitangent fields, and RemoveTangents to remove both fields. Use
		 * GetBitangents to retrieve the bitangent field.
		 */
		virtual	SGDEPRECATED void AddBitangents( rid level ) = 0;

		/**
		 * \deprecated GetUserTriangleVertexField() is deprecated, use GetUserCornerField()
		 * instead.AddUserTriangleVertexField() is renamed AddUserCornerField() RemoveUserTriangleVertexField()
		 * is renamed RemoveUserCornerField() GetUserTriangleVertexField is renamed GetUserCornerField()
		 */
		virtual	SGDEPRECATED CountedPointer<IValueArray> GetUserTriangleVertexField( const char * name ) = 0;

		/**
		 * \deprecated RemoveBitangents() is deprecated, use RemoveTangents() instead.Bitangents
		 * are Added and Removed in unison with Tangents. Only need  to call AddTangents to
		 * add both Tangent and Bitangent fields, and RemoveTangents to remove both fields.
		 * Use GetBitangents to retrieve the bitangent field.
		 */
		virtual	SGDEPRECATED void RemoveBitangents( rid level ) = 0;

		/**
		 * \deprecated AddDiffuseColors() is deprecated, use AddColors() instead.DiffuseColors
		 * is now an alias for 'Colors0'
		 */
		virtual	SGDEPRECATED void AddDiffuseColors(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified corner field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing corner fields.
		 */
		virtual	void AddUserCornerField( IValueArray *field ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetBitangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets the Normals field in the corners.  (Field name: "Normals" , Tuple
		 * size: 3 , Stored as: Corner attribute)
		 */
		virtual	void AddNormals(  ) = 0;

		/**
		 * Copies all fields in the specified corner in the source geometry to this geometrys'
		 * corner. CopyCombineCorner combines two triangles by linear interpolation into the
		 * destination triangle. CopyCombine3Corner combines three triangles via barycentric
		 * coordinates.
		 */
		virtual	void CopyCombineCorners( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * \deprecated AddSpecularColors() is deprecated, use AddColors() instead.SpecularColors
		 * is now an alias for 'Colors1'
		 */
		virtual	SGDEPRECATED void AddSpecularColors(  ) = 0;

		/**
		 * Creates another geometry data object with the same field setup.  To also copy the
		 * data to the new object, set copy_data to true.
		 */
		virtual	CountedPointer<IGeometryData> NewCopy( bool copy_data ) = 0;

		/**
		 * Adds/Removes/Gets the MaterialIds field in the triangles. (Field name: "MaterialIds",
		 * Tuple size: 1 , Stored as: triangle attribute)
		 */
		virtual	CountedPointer<IRidArray> GetMaterialIds(  ) = 0;

		/**
		 * Retrieves the ReFieldData object that contains the triangle fields
		 */
		virtual	CountedPointer<IFieldData> GetTriangles(  ) = 0;

		/**
		 * Adds/Removes/Gets a custom field. To be able to add a field, it must have a name
		 * that does not  conflict with the existing custom fields.
		 */
		virtual	void AddCustomField( IValueArray *field ) = 0;

		/**
		 * \deprecated GetTriangleVertices() is deprecated, use GetCorners() instead.GetTriangleVertices()
		 * is renamed GetCorners()
		 */
		virtual	SGDEPRECATED CountedPointer<IFieldData> GetTriangleVertices(  ) = 0;

		/**
		 * \deprecated CopyCombineTriangleVertices() is deprecated, use CopyCombineCorners()
		 * instead.CopyTriangleVertex() is renamed CopyCorner() CopyCombineTriangleVertices()
		 * is renamed CopyCombineCorners() CopyCombine3TriangleVertices() is renamed CopyCombine3Corners()
		 */
		virtual	SGDEPRECATED void CopyCombineTriangleVertices( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , real alpha ) = 0;

		/**
		 * \deprecated CopyCombine3TriangleVertices() is deprecated, use CopyCombine3Corners()
		 * instead.CopyTriangleVertex() is renamed CopyCorner() CopyCombineTriangleVertices()
		 * is renamed CopyCombineCorners() CopyCombine3TriangleVertices() is renamed CopyCombine3Corners()
		 */
		virtual	SGDEPRECATED void CopyCombine3TriangleVertices( IGeometryData *source , rid dest_id , rid src_id_1 , rid src_id_2 , rid src_id_3 , real alpha_1 , real alpha_2 ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGeometryData
	 */
	typedef CountedPointer<IGeometryData> spGeometryData;

	class IPackedGeometryData;
	class IGeometryData;
	class IRealArray;
	class IBoolArray;
	class IRidArray;
	class IValueArray;
	class IFieldData;

	/**
	 * RePackedGeometryData keeps the same information as ReGeometryData, but with all corner
	 * data fields moved into per-vertex fields. No per-corner fields exist, apart from
	 * the VertexIds field.  Please note that the number of vertices in an RePackedGeometryData
	 * is commonly higher than in an ReGeometryData, as vertices must be split to accommodate
	 * for different corner data.
	 */
	class IPackedGeometryData : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IPackedGeometryData is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IPackedGeometryData",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IPackedGeometryData pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IPackedGeometryData pointer
		 * @return a pointer to the IPackedGeometryData object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IPackedGeometryData *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IPackedGeometryData") )
				return static_cast<IPackedGeometryData*>(ptr);
			return NULL;
			}
		/**
		 * Adds/Removes/Gets the VertexWeighting field for the vertices. (Field name: "VertexWeighting",
		 * Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveVertexWeighting(  ) = 0;

		/**
		 * \deprecated RemoveSpecularColors() is deprecated, use RemoveColors() instead.SpecularColors
		 * is now an alias for 'Colors1'
		 */
		virtual	SGDEPRECATED void RemoveSpecularColors(  ) = 0;

		/**
		 * Adds/Removes/Gets the Normals field in the vertices.  (Field name: "Normals" , Tuple
		 * size: 3 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetNormals(  ) = 0;

		/**
		 * Adds/Removes/Gets the GroupIds field in the triangles. (Field name: "GroupIds", Tuple
		 * size: 1 , Stored as: triangle attribute)
		 */
		virtual	CountedPointer<IRidArray> GetGroupIds(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	void AddUserTriangleField( IValueArray *field ) = 0;

		/**
		 * Retrieves the ReFieldData object that contains the triangle fields
		 */
		virtual	CountedPointer<IFieldData> GetTriangles(  ) = 0;

		/**
		 * Gets the VertexIds field in the triangles.
		 */
		virtual	CountedPointer<IRidArray> GetVertexIds(  ) = 0;

		/**
		 * Set/Get the inferior (minimum) extent of the geometry
		 */
		virtual	void SetInf( real vec[3] ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetBitangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a TexCoords field in the vertices.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'TexCoords0' - 'TexCoords15' (Field name:
		 * "TexCoords0" - "TexCoords15", Tuple size: 2 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveTexCoords( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a TexCoords field in the vertices.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'TexCoords0' - 'TexCoords15' (Field name:
		 * "TexCoords0" - "TexCoords15", Tuple size: 2 , Stored as: Vertex attribute)
		 */
		virtual	void AddTexCoords( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	CountedPointer<IValueArray> GetUserTriangleField( const char * name ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	void AddBoneWeights( unsigned int tuplesize ) = 0;

		/**
		 * Adds/Removes/Gets a Color field in the vertices.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'Colors0' - 'Colors15' (Field name: "Colors0"
		 * - "Colors15", Tuple size: 3 , Stored as: Vertex attribute)
		 */
		virtual	void AddColors( rid level ) = 0;

		/**
		 * Adds/Removes/Gets the GroupIds field in the triangles. (Field name: "GroupIds", Tuple
		 * size: 1 , Stored as: triangle attribute)
		 */
		virtual	void AddGroupIds(  ) = 0;

		/**
		 * Adds/Removes/Gets a Color field in the vertices.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'Colors0' - 'Colors15' (Field name: "Colors0"
		 * - "Colors15", Tuple size: 3 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveColors( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeUserTriangleField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Adds/Removes/Gets the VertexWeighting field for the vertices. (Field name: "VertexWeighting",
		 * Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void AddVertexWeighting(  ) = 0;

		/**
		 * Set/Get the superior (maximum) extent of the geometry
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Set/Get the superior (maximum) extent of the geometry
		 */
		virtual	void SetSup( real vec[3] ) = 0;

		/**
		 * \deprecated AddSpecularColors() is deprecated, use AddColors() instead.SpecularColors
		 * is now an alias for 'Colors1'
		 */
		virtual	SGDEPRECATED void AddSpecularColors(  ) = 0;

		/**
		 * Adds/Removes/Gets the GroupIds field in the triangles. (Field name: "GroupIds", Tuple
		 * size: 1 , Stored as: triangle attribute)
		 */
		virtual	void RemoveGroupIds(  ) = 0;

		/**
		 * Adds/Removes/Gets the VertexWeighting field for the vertices. (Field name: "VertexWeighting",
		 * Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetVertexWeighting(  ) = 0;

		/**
		 * \deprecated RemoveBitangents() is deprecated, use RemoveTangents() instead.Bitangents
		 * are Added and Removed in unison with Tangents. Only need  to call AddTangents to
		 * add both Tangent and Bitangent fields, and RemoveTangents to remove both fields.
		 * Use GetBitangents to retrieve the bitangent field.
		 */
		virtual	SGDEPRECATED void RemoveBitangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	CountedPointer<IValueArray> AddBaseTypeUserVertexField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Creates an "unpacked" ReGeometryData object. Please note that since the data in the
		 * RePackedGeometryData structure is per-vertex, you may have to weld the resulting
		 * ReGeometryData to remove vertices with the same spatial coordinates.
		 */
		virtual	CountedPointer<IGeometryData> NewUnpackedCopy(  ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	void RemoveBoneWeights(  ) = 0;

		/**
		 * Gets/Sets the number of triangles in the geometry. AddTriangles increases the triangle
		 * count by the specified value.
		 */
		virtual	void SetTriangleCount( unsigned int count ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	CountedPointer<IRidArray> GetBoneIds(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified triangle field. To be able to add a field, it
		 * must have a name that does not  conflict with the existing triangle fields.
		 */
		virtual	void RemoveUserTriangleField( const char * name ) = 0;

		/**
		 * Set/Get the inferior (minimum) extent of the geometry
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

		/**
		 * Adds/Removes/Gets the VertexLocks field for the vertices. If the value for a vertex
		 * is true, then the vertex should be locked,  and not be removed in the collapse-process.
		 * (Field name: "VertexLocks", Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IBoolArray> GetVertexLocks(  ) = 0;

		/**
		 * Copies the field setup and data from another object. To only copy the setup, set
		 * copy_data to false.
		 */
		virtual	void DeepCopy( IPackedGeometryData *source , bool copy_data ) = 0;

		/**
		 * Adds/Removes/Gets the Normals field in the vertices.  (Field name: "Normals" , Tuple
		 * size: 3 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveNormals(  ) = 0;

		/**
		 * Creates another geometry data object with the same field setup.  To also copy the
		 * data to the new object, set copy_data to true.
		 */
		virtual	CountedPointer<IPackedGeometryData> NewCopy( bool copy_data ) = 0;

		/**
		 * Adds/Removes/Gets the VertexLocks field for the vertices. If the value for a vertex
		 * is true, then the vertex should be locked,  and not be removed in the collapse-process.
		 * (Field name: "VertexLocks", Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void RemoveVertexLocks(  ) = 0;

		/**
		 * Adds/Removes/Gets the Normals field in the vertices.  (Field name: "Normals" , Tuple
		 * size: 3 , Stored as: Vertex attribute)
		 */
		virtual	void AddNormals(  ) = 0;

		/**
		 * \deprecated AddDiffuseColors() is deprecated, use AddColors() instead.DiffuseColors
		 * is now an alias for 'Colors0'
		 */
		virtual	SGDEPRECATED void AddDiffuseColors(  ) = 0;

		/**
		 * \deprecated GetDiffuseColors() is deprecated, use GetColors() instead.DiffuseColors
		 * is now an alias for 'Colors0'
		 */
		virtual	SGDEPRECATED CountedPointer<IRealArray> GetDiffuseColors(  ) = 0;

		/**
		 * \deprecated RemoveDiffuseColors() is deprecated, use RemoveColors() instead.DiffuseColors
		 * is now an alias for 'Colors0'
		 */
		virtual	SGDEPRECATED void RemoveDiffuseColors(  ) = 0;

		/**
		 * Adds/Removes/Gets a Color field in the vertices.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'Colors0' - 'Colors15' (Field name: "Colors0"
		 * - "Colors15", Tuple size: 3 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetColors( rid level ) = 0;

		/**
		 * \deprecated GetSpecularColors() is deprecated, use GetColors() instead.SpecularColors
		 * is now an alias for 'Colors1'
		 */
		virtual	SGDEPRECATED CountedPointer<IRealArray> GetSpecularColors(  ) = 0;

		/**
		 * Gets/Sets the number of vertices in the geometry. AddVertices increases the vertex
		 * count by the specified value.
		 */
		virtual	void SetVertexCount( unsigned int count ) = 0;

		/**
		 * Gets/Sets the number of vertices in the geometry. AddVertices increases the vertex
		 * count by the specified value.
		 */
		virtual	unsigned int GetVertexCount(  ) = 0;

		/**
		 * \deprecated AddBitangents() is deprecated, use AddTangents() instead.Bitangents are
		 * Added and Removed in unison with Tangents. Only need  to call AddTangents to add
		 * both Tangent and Bitangent fields, and RemoveTangents to remove both fields. Use
		 * GetBitangents to retrieve the bitangent field.
		 */
		virtual	SGDEPRECATED void AddBitangents( rid level ) = 0;

		/**
		 * Gets/Sets the number of triangles in the geometry. AddTriangles increases the triangle
		 * count by the specified value.
		 */
		virtual	unsigned int GetTriangleCount(  ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	void RemoveUserVertexField( const char * name ) = 0;

		/**
		 * Gets/Sets the number of vertices in the geometry. AddVertices increases the vertex
		 * count by the specified value.
		 */
		virtual	void AddVertices( unsigned int count ) = 0;

		/**
		 * Gets/Sets the number of triangles in the geometry. AddTriangles increases the triangle
		 * count by the specified value.
		 */
		virtual	void AddTriangles( unsigned int count ) = 0;

		/**
		 * Gets the Coords field in the vertices. (Field name: "Coords", Tuple size: 3 , Stored
		 * as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetCoords(  ) = 0;

		/**
		 * Adds/Removes/Gets a TexCoords field in the vertices.  The valid id range of the level
		 * parameter is 0-15, which equals fields  'TexCoords0' - 'TexCoords15' (Field name:
		 * "TexCoords0" - "TexCoords15", Tuple size: 2 , Stored as: Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetTexCoords( rid level ) = 0;

		/**
		 * Adds/Removes/Gets the BoneWeights and BoneIds fields in the vertices. The BoneWeights
		 * and BoneIds fields are added and removed in unison, and  should always be the same
		 * tuple size. (Field name: "BoneWeight & BoneIds", Tuple size: varying , Stored as:
		 * Vertex attribute)
		 */
		virtual	CountedPointer<IRealArray> GetBoneWeights(  ) = 0;

		/**
		 * Adds/Removes/Gets the VertexLocks field for the vertices. If the value for a vertex
		 * is true, then the vertex should be locked,  and not be removed in the collapse-process.
		 * (Field name: "VertexLocks", Tuple size: 1 , Stored as: Vertex attribute)
		 */
		virtual	void AddVertexLocks(  ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	void AddTangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	void RemoveTangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets the MaterialIds field in the triangles. (Field name: "MaterialIds",
		 * Tuple size: 1 , Stored as: triangle attribute)
		 */
		virtual	void RemoveMaterialIds(  ) = 0;

		/**
		 * Adds/Removes/Gets a Tangents field in the corners. The tangents are stored in Simplygon
		 * as full tangent fields, using both Tangent and Bitangent fields, both 3d vectors.
		 * (Field name: "Tangents" & "Bitangents", Tuple size: 3 , Stored as: Corner attribute)
		 */
		virtual	CountedPointer<IRealArray> GetTangents( rid level ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	void AddUserVertexField( IValueArray *field ) = 0;

		/**
		 * Adds/Removes/Gets a user-specified vertex field. To be able to add a field, it must
		 * have a name that does not  conflict with the existing vertex fields.
		 */
		virtual	CountedPointer<IValueArray> GetUserVertexField( const char * name ) = 0;

		/**
		 * Retrieves the ReFieldData object that contains the vertex fields
		 */
		virtual	CountedPointer<IFieldData> GetVertices(  ) = 0;

		/**
		 * Adds/Removes/Gets the MaterialIds field in the triangles. (Field name: "MaterialIds",
		 * Tuple size: 1 , Stored as: triangle attribute)
		 */
		virtual	void AddMaterialIds(  ) = 0;

		/**
		 * Adds/Removes/Gets the MaterialIds field in the triangles. (Field name: "MaterialIds",
		 * Tuple size: 1 , Stored as: triangle attribute)
		 */
		virtual	CountedPointer<IRidArray> GetMaterialIds(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IPackedGeometryData
	 */
	typedef CountedPointer<IPackedGeometryData> spPackedGeometryData;

	class IImageData;
	class IValueArray;
	class IFieldData;

	/**
	 * ReImageData holds unpacked image data. The data is  stored in an ReFieldData object
	 * as value fields. For simplicity all  images are implemented as 3D images, but has
	 * an extent of 1 in Z for 2D images, and an extent of 1 in both Y and Z for 1D images.
	 * Cube maps are stored as a 3D image with Z-depth of 6 (one for each side)
	 */
	class IImageData : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IImageData is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IImageData",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IImageData pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IImageData pointer
		 * @return a pointer to the IImageData object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IImageData *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IImageData") )
				return static_cast<IImageData*>(ptr);
			return NULL;
			}
		/**
		 * Creates another image data object with the same field setup.  To also copy the data
		 * to the new object, set copy_data to true. @param copy_data if true, all data is copied
		 * @return the new copy of the image data object
		 */
		virtual	CountedPointer<IImageData> NewCopy( bool copy_data ) = 0;

		/**
		 * Clears all data from the image, and removes all data fields. The type is reset to
		 * 2D
		 */
		virtual	void Reset(  ) = 0;

		/**
		 * Removes the Colors field. 
		 */
		virtual	void RemoveColors(  ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	unsigned int GetYSize(  ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	unsigned int GetType(  ) = 0;

		/**
		 * Gets the Colors field.  @return the value array used by the colors field.
		 */
		virtual	CountedPointer<IValueArray> GetColors(  ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	void Set1DSize( unsigned int _xs ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	void Set2DSize( unsigned int _xs , unsigned int _ys ) = 0;

		/**
		 * Adds/Removes/Gets a field. The field must have a name that does not conflict with
		 * any of the existsing fields' names. The field is referenced by the image after the
		 * call to AddField
		 */
		virtual	void RemoveField( const char * name ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	void Set3DSize( unsigned int _xs , unsigned int _ys , unsigned int _zs ) = 0;

		/**
		 * Copies the field setup and data from another object. To only copy the setup, set
		 * copy_data to false.
		 */
		virtual	void DeepCopy( IImageData *source , bool copy_data ) = 0;

		/**
		 * Adds/Removes/Gets a field. The field must have a name that does not conflict with
		 * any of the existsing fields' names. The field is referenced by the image after the
		 * call to AddField
		 */
		virtual	CountedPointer<IValueArray> GetField( const char * name ) = 0;

		/**
		 * Adds/Removes/Gets a field. The field must have a name that does not conflict with
		 * any of the existsing fields' names. The field is referenced by the image after the
		 * call to AddField
		 */
		virtual	void AddField( const char * name , rid valuetype , unsigned int tuplesize ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	void SetCubeSize( unsigned int _xs , unsigned int _ys ) = 0;

		/**
		 * Checks if the image has a color field @return true if the image has a color field
		 */
		virtual	bool HasColors(  ) = 0;

		/**
		 * Get the ImageFields field data object, that holds all fields within the image
		 */
		virtual	CountedPointer<IFieldData> GetImageFields(  ) = 0;

		/**
		 * Gets the color field format specification @return the format specification string
		 */
		virtual	rstring GetColorsFormat(  ) = 0;

		/**
		 * Adds the Colors field.  @param valuetype is the type used to store the tuples.  
		 * Use TYPES_ID_UCHAR base type for 8-bit per channel data, and TYPES_ID_USHORT base
		 * type for 16-bit per channel data.  @param format is a string that describes the format
		 * of the color field. each character in format a color channel in the color tuple.
		 * all characters in the format string must be unique and uppercase. normal values for
		 * format are: RGB, RGBA, YUV, HLS, CMYK, L and R. In theese  examples the tuplesize
		 * of the formats will be 3,4,3,3,4,1  and 1, respectively. the format string must be
		 * at least one  character long. Use the SG_IMAGEDATA_FORMAT_[] defines to  define the
		 * standard types.
		 */
		virtual	void AddColors( rid valuetype , const char * format ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	unsigned int GetZSize(  ) = 0;

		/**
		 * Gets the base type used by the colors field @return the base type used by the colors
		 * field.
		 */
		virtual	rid GetColorsValueType(  ) = 0;

		/**
		 * Adds/Removes/Gets a field. The field must have a name that does not conflict with
		 * any of the existsing fields' names. The field is referenced by the image after the
		 * call to AddField
		 */
		virtual	rid GetFieldValueType( const char * name ) = 0;

		/**
		 * Adds/Removes/Gets a field. The field must have a name that does not conflict with
		 * any of the existsing fields' names. The field is referenced by the image after the
		 * call to AddField
		 */
		virtual	bool HasField( const char * name ) = 0;

		/**
		 * Set/Get the dimensions and type of the image data.
		 */
		virtual	unsigned int GetXSize(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IImageData
	 */
	typedef CountedPointer<IImageData> spImageData;


	/**
	 * ReTable is the base class for tables objects, where items are added into an id-indexed
	 * array of objects. 
	 */
	class ITable : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ITable is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ITable",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ITable pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ITable pointer
		 * @return a pointer to the ITable object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ITable *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ITable") )
				return static_cast<ITable*>(ptr);
			return NULL;
			}
		/**
		 * Sets an item in the table. The id must exist in the table.
		 */
		virtual	void SetItem( rid id , IObject *item ) = 0;

		/**
		 * Adds an item to the table. The returned value is  the id of the item within the table,
		 * which can be used to retrieve the item using GetItem(). 
		 */
		virtual	rid AddItem( IObject *item ) = 0;

		/**
		 * Finds a named item in the table. If multiple items  have the same name, the first
		 * will be returned. If no item  was found, the return will be NULL.
		 */
		virtual	CountedPointer<IObject> FindItem( const char * name ) = 0;

		/**
		 * Finds the id of an item in the table, using the name. If multiple items have the
		 * same name, the first will be returned. If no item was found, the return will be -1.
		 */
		virtual	rid FindItemId( const char * name ) = 0;

		/**
		 * Returns the i:th item, using the id of the item
		 */
		virtual	CountedPointer<IObject> GetItem( rid id ) = 0;

		/**
		 * Removes an item, and invalidates the id. The id will not point at a valid item, until
		 * Clear is called and new items are added up until the id.
		 */
		virtual	void RemoveItem( rid id ) = 0;

		/**
		 * Clears the table, and removes all items. If AddItem is called after Clear, the item
		 * ids will start over from 0
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the number of item id:s in the table. If an item has been removed, using
		 * RemoveItem, the id is still  counted in this method. GetIdsCount also equals the
		 * id that will be applied to the next item added to the table.
		 */
		virtual	unsigned int GetItemsCount(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ITable
	 */
	typedef CountedPointer<ITable> spTable;

	class IImageData;
	class IMaterial;

	/**
	 * ReMaterial keeps information on materials used while rendering.
	 */
	class IMaterial : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IMaterial is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IMaterial",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IMaterial pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IMaterial pointer
		 * @return a pointer to the IMaterial object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IMaterial *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IMaterial") )
				return static_cast<IMaterial*>(ptr);
			return NULL;
			}
		/**
		 * Get a named texture texture coordinate level The valid values are 0->15 @param channel_name
		 * the name of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @returns the texture coordinate level
		 */
		virtual	rid GetTextureLevel( const char * channel_name ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	void SetDiffuseRed( real val ) = 0;

		/**
		 * Get a named texture image data. @param channel_name the name of the material channel.
		 * Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @returns the
		 * texture object
		 */
		virtual	CountedPointer<IImageData> GetTextureImage( const char * channel_name ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	void SetDiffuseGreen( real val ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	void SetAmbientGreen( real val ) = 0;

		/**
		 * Add/Remove a user material channel, or check if the user channel exists within the
		 * material.
		 */
		virtual	void AddUserChannel( const char * channel_name ) = 0;

		/**
		 * Get the base color of the material channel.  @param channel_name the name of the
		 * material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines
		 * @returns the value of the component
		 */
		virtual	real GetColorAlpha( const char * channel_name ) = 0;

		/**
		 * Add/Remove a user material channel, or check if the user channel exists within the
		 * material.
		 */
		virtual	void RemoveUserChannel( const char * channel_name ) = 0;

		/**
		 * Get the base color of the material channel.  @param channel_name the name of the
		 * material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines
		 * @returns the value of the component
		 */
		virtual	real GetColorBlue( const char * channel_name ) = 0;

		/**
		 * Get the base color of the material channel.  @param channel_name the name of the
		 * material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines
		 * @returns the value of the component
		 */
		virtual	real GetColorGreen( const char * channel_name ) = 0;

		/**
		 * Set/Get Displacement multiplier
		 */
		virtual	void SetDisplacement( real value ) = 0;

		/**
		 * Add/Remove a user material channel, or check if the user channel exists within the
		 * material.
		 */
		virtual	bool HasUserChannel( const char * channel_name ) = 0;

		/**
		 * Set the base color of the material channel.  @param channel_name the name of the
		 * material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines
		 * @param r the red value  @param g the green value  @param b the blue value  @param
		 * a the alpha value 
		 */
		virtual	void SetColor( const char * channel_name , real r , real g , real b , real a ) = 0;

		/**
		 * Set a named texture image data. @param channel_name the name of the material channel.
		 * Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param image
		 * the image object
		 */
		virtual	void SetTextureImage( const char * channel_name , IImageData *image ) = 0;

		/**
		 * Set a named texture texture coordinate level The valid values are 0->15 @param channel_name
		 * the name of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @param level the texture coordinate level
		 */
		virtual	void SetTextureLevel( const char * channel_name , rid level ) = 0;

		/**
		 * Set/Get Displacement multiplier
		 */
		virtual	real GetDisplacement(  ) = 0;

		/**
		 * Set/Get the TangentSpaceNormals flag. If set, the normal map has tangent space normals
		 */
		virtual	void SetTangentSpaceNormals( bool value ) = 0;

		/**
		 * Set the vertex color channel used for the material channel.  @param channel_name
		 * the name of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @param vertex_color_channel_id the vertex color channel to use for the material
		 * channel, or -1 to not use the current vertex color channel
		 */
		virtual	void SetVertexColorChannel( const char * channel_name , rid vertex_color_channel_id ) = 0;

		/**
		 * Set a named texture filename. If a texture image is set using SetTextureImage/SetLayeredTextureImage
		 * , this parameter is not used to load texture data. @param channel_name the name of
		 * the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @param texture_filename the texture file name
		 */
		virtual	void SetTexture( const char * channel_name , const char * texture_filename ) = 0;

		/**
		 * Returns true if the material is setup identical to this material
		 */
		virtual	bool IsIdenticalTo( IMaterial *mat ) = 0;

		/**
		 * Get the vertex color channel used for the material channel.  @param channel_name
		 * the name of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @returns the vertex color channel, or -1 if no channel is set for the material
		 * channel
		 */
		virtual	rid GetVertexColorChannel( const char * channel_name ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	real GetAmbientBlue(  ) = 0;

		/**
		 * Returns the name of a channel from the index. Note that by adding/removing channels
		 * the indices are changed, and this method should only be used for enumeration
		 */
		virtual	rstring GetChannelFromIndex( rid index ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	void SetAmbientBlue( real val ) = 0;

		/**
		 * Set the base color of the material channel.  @param channel_name the name of the
		 * material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines
		 * @param r the red value  @param g the green value  @param b the blue value 
		 */
		virtual	void SetColorRGB( const char * channel_name , real r , real g , real b ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	real GetSpecularGreen(  ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	void SetDiffuseBlue( real val ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	real GetDiffuseRed(  ) = 0;

		/**
		 * Changes the prefix path (drive, directory) of all matching texture paths in the material,
		 * with the new one.  This method is useful when external textures are placed in a specific
		 * folder, which needs to be moved @param current_path_prefix the current prefix path,
		 * either relative or absolute @param new_path_prefix the new prefix path
		 */
		virtual	void ChangeTexturePrefixPath( const char * current_path_prefix , const char * new_path_prefix ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	real GetSpecularBlue(  ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	real GetAmbientRed(  ) = 0;

		/**
		 * Set/Get Shininess multiplier (0->128)
		 */
		virtual	void SetShininess( real value ) = 0;

		/**
		 * Set/Get the diffuse transparency override. If set, IsTransparent will always return
		 * true.
		 */
		virtual	void SetDiffuseTextureHasAlpha( bool value ) = 0;

		/**
		 * Get a named texture filename. If a texture image is set using SetTextureImage/SetLayeredTextureImage
		 * , this parameter is not used to load texture data. @param channel_name the name of
		 * the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @returns the texture file name
		 */
		virtual	rstring GetTexture( const char * channel_name ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	real GetDiffuseGreen(  ) = 0;

		/**
		 * Set the number of textures maps in a layered texture @param channel_name the name
		 * of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @param size the number of textures in the layered texture (>=1)
		 */
		virtual	void SetLayeredTextureSize( const char * channel_name , unsigned int size ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	real GetDiffuseBlue(  ) = 0;

		/**
		 * Get the number of textures maps in a layered texture @param channel_name the name
		 * of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @returns the number of textures in the layered texture (>=1)
		 */
		virtual	unsigned int GetLayeredTextureSize( const char * channel_name ) = 0;

		/**
		 * Set the blend type of a texture in a layered texture. Note that the SetTextureBlendType
		 * always sets the first texture (layer id 0) @param channel_name the name of the material
		 * channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param
		 * layer_id the layer id (0->size-1) @param blend_type the blend type to set
		 */
		virtual	void SetLayeredTextureBlendType( const char * channel_name , rid layer_id , rid blend_type ) = 0;

		/**
		 * Set the texture coordinate level of a texture in a layered texture. Note that the
		 * SetTextureLevel always sets the first texture (layer id 0) @param channel_name the
		 * name of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @param layer_id the layer id (0->size-1) @param level the texture coordinate
		 * level
		 */
		virtual	void SetLayeredTextureLevel( const char * channel_name , rid layer_id , rid level ) = 0;

		/**
		 * Set/Get the TangentSpaceNormals flag. If set, the normal map has tangent space normals
		 */
		virtual	bool GetTangentSpaceNormals(  ) = 0;

		/**
		 * Get/Set the Diffuse Color of the material
		 */
		virtual	void SetDiffuseColor( real r , real g , real b ) = 0;

		/**
		 * Set a named texture filename in a layered texture. Note that the SetTexture always
		 * sets the first texture (layer id 0) @param channel_name the name of the material
		 * channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param
		 * layer_id the layer id (0->size-1) @param texture_filename the texture file name
		 */
		virtual	void SetLayeredTexture( const char * channel_name , rid layer_id , const char * texture_filename ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	void SetSpecularRed( real val ) = 0;

		/**
		 * Get a named texture filename in a layered texture. Note that the SetTexture always
		 * sets the first texture (layer id 0) @param channel_name the name of the material
		 * channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param
		 * layer_id the layer id (0->size-1) @returns the texture file name
		 */
		virtual	rstring GetLayeredTexture( const char * channel_name , rid layer_id ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	void SetSpecularColor( real r , real g , real b ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	void SetSpecularGreen( real val ) = 0;

		/**
		 * Get the base color of the material channel.  @param channel_name the name of the
		 * material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines
		 * @returns the value of the component
		 */
		virtual	real GetColorRed( const char * channel_name ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	void SetSpecularBlue( real val ) = 0;

		/**
		 * Set/Get opacity of material (0->1)
		 */
		virtual	void SetOpacity( real value ) = 0;

		/**
		 * Get/Set the Specular Color of the material
		 */
		virtual	real GetSpecularRed(  ) = 0;

		/**
		 * Returns the number of material channels in the material
		 */
		virtual	unsigned int GetChannelCount(  ) = 0;

		/**
		 * Set a named texture image data in a layered texture. Note that the SetTextureImage
		 * always sets the first texture (layer id 0) @param channel_name the name of the material
		 * channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param
		 * layer_id the layer id (0->size-1) @param image the image object
		 */
		virtual	void SetLayeredTextureImage( const char * channel_name , rid layer_id , IImageData *image ) = 0;

		/**
		 * Get the blend type of a texture in a layered texture. Note that the SetTextureBlendType
		 * always sets the first texture (layer id 0) @param channel_name the name of the material
		 * channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param
		 * layer_id the layer id (0->size-1) @returns the blend type for the texture
		 */
		virtual	rid GetLayeredTextureBlendType( const char * channel_name , rid layer_id ) = 0;

		/**
		 * Set/Get the diffuse transparency override. If set, IsTransparent will always return
		 * true.
		 */
		virtual	bool GetDiffuseTextureHasAlpha(  ) = 0;

		/**
		 * Get a named texture image data in a layered texture. Note that the SetTextureImage
		 * always sets the first texture (layer id 0) @param channel_name the name of the material
		 * channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...] defines @param
		 * layer_id the layer id (0->size-1) @returns the texture object
		 */
		virtual	CountedPointer<IImageData> GetLayeredTextureImage( const char * channel_name , rid layer_id ) = 0;

		/**
		 * Get the texture coordinate level of a texture in a layered texture. Note that the
		 * SetTextureLevel always sets the first texture (layer id 0) @param channel_name the
		 * name of the material channel. Default channels are defined in the SG_MATERIAL_CHANNEL_[...]
		 * defines @param layer_id the layer id (0->size-1) @returns the texture coordinate
		 * level
		 */
		virtual	rid GetLayeredTextureLevel( const char * channel_name , rid layer_id ) = 0;

		/**
		 * Set/Get opacity of material (0->1)
		 */
		virtual	real GetOpacity(  ) = 0;

		/**
		 * Returns true if the material is transparent, either by diffuse color, if there is
		 * an opacity texture, or the diffuse texture has an alpha channel
		 */
		virtual	bool IsTransparent(  ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	void SetAmbientRed( real val ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	real GetAmbientGreen(  ) = 0;

		/**
		 * Get/Set the Ambient Color of the material
		 */
		virtual	void SetAmbientColor( real r , real g , real b ) = 0;

		/**
		 * Set/Get Shininess multiplier (0->128)
		 */
		virtual	real GetShininess(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IMaterial
	 */
	typedef CountedPointer<IMaterial> spMaterial;

	class IMaterial;

	/**
	 * ReMaterialTable keeps information on materials used while rendering.
	 */
	class IMaterialTable : public ITable
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IMaterialTable is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IMaterialTable",type)==0 )
				return true;
			return ITable::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IMaterialTable pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IMaterialTable pointer
		 * @return a pointer to the IMaterialTable object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IMaterialTable *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IMaterialTable") )
				return static_cast<IMaterialTable*>(ptr);
			return NULL;
			}
		/**
		 * Returns the number of item id:s in the table. If an item has been removed, using
		 * RemoveItem, the id is still  counted in this method. GetIdsCount also equals the
		 * id that will be applied to the next item added to the table.
		 */
		virtual	unsigned int GetItemsCount(  ) = 0;

		/**
		 * Returns the i:th material, using the id of the material @param id the id of the material
		 * in the table @return the the material in the table
		 */
		virtual	CountedPointer<IMaterial> GetMaterial( rid id ) = 0;

		/**
		 * Removes an item, and invalidates the id. The id will not point at a valid item, until
		 * Clear is called and new items are added up until the id.
		 */
		virtual	void RemoveItem( rid id ) = 0;

		/**
		 * Sets an item in the table. The id must exist in the table.
		 */
		virtual	void SetItem( rid id , IObject *item ) = 0;

		/**
		 * Finds a named item in the table. If multiple items  have the same name, the first
		 * will be returned. If no item  was found, the return will be NULL.
		 */
		virtual	CountedPointer<IObject> FindItem( const char * name ) = 0;

		/**
		 * Removes a material. If GetMaterial is called with the material id, the value returned
		 * will be NULL. @param id the id of the material in the table
		 */
		virtual	void RemoveMaterial( rid id ) = 0;

		/**
		 * Finds a material in the table, using the material name. If multiple materials have
		 * the same name, the first will be returned. If the material was not found, the return
		 * will be NULL. @param name the material name to look for @return the material, if
		 * found, NULL otherwise
		 */
		virtual	CountedPointer<IMaterial> FindMaterial( const char * name ) = 0;

		/**
		 * Returns the number of materials in the table
		 */
		virtual	unsigned int GetMaterialsCount(  ) = 0;

		/**
		 * Finds a material in the table that is setup identical to the specified material.
		 * @param mat the material setup to look for @return the material, if found, NULL otherwise
		 */
		virtual	CountedPointer<IMaterial> FindIdenticalMaterial( IMaterial *mat ) = 0;

		/**
		 * Finds the id of a material in the table that is setup identical to the specified
		 * material. @param mat the material setup to look for @return the material id if found,
		 * -1 otherwise
		 */
		virtual	rid FindIdenticalMaterialId( IMaterial *mat ) = 0;

		/**
		 * Changes the prefix path (drive, directory) of all matching texture paths in all the
		 * materials in the table, with the new one.  This method is useful when external textures
		 * are placed in a specific folder, which needs to be moved @param current_path_prefix
		 * the current prefix path, either relative or absolute @param new_path_prefix the new
		 * prefix path		
		 */
		virtual	void ChangeTexturePrefixPath( const char * current_path_prefix , const char * new_path_prefix ) = 0;

		/**
		 * Finds the id of a material in the table, using the material name. If multiple materials
		 * have the same name, the first will be returned. If the material was not found, the
		 * return will be -1. @param name the material name to look for @return the material
		 * id if found, -1 otherwise
		 */
		virtual	rid FindMaterialId( const char * name ) = 0;

		/**
		 * Finds the id of an item in the table, using the name. If multiple items have the
		 * same name, the first will be returned. If no item was found, the return will be -1.
		 */
		virtual	rid FindItemId( const char * name ) = 0;

		/**
		 * Adds an item to the table. The returned value is  the id of the item within the table,
		 * which can be used to retrieve the item using GetItem(). 
		 */
		virtual	rid AddItem( IObject *item ) = 0;

		/**
		 * Returns the i:th item, using the id of the item
		 */
		virtual	CountedPointer<IObject> GetItem( rid id ) = 0;

		/**
		 * Clears the table, and removes all items. If AddItem is called after Clear, the item
		 * ids will start over from 0
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Adds a material to the table. The returned value is  the id of the material within
		 * the table. @param material the material to add into the table @return the material
		 * id of the material in the table
		 */
		virtual	rid AddMaterial( IMaterial *material ) = 0;

		/**
		 * Sets a material in the table.  @param id the id of the material in the table @param
		 * material the material to set into the table
		 */
		virtual	void SetMaterial( rid id , IMaterial *material ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IMaterialTable
	 */
	typedef CountedPointer<IMaterialTable> spMaterialTable;

	class IMatrix4x4;
	class IRealArray;

	/**
	 * ReMatrix4x4 is used to represent and manipulate 4x4  transformation matrices, which
	 * can be either standard matrixes, or  homogeneous 4x4 matrices used to transform 3D
	 * homogeneous coordinates  [x y z w] The transformations are defined in row-major order.
	 */
	class IMatrix4x4 : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IMatrix4x4 is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IMatrix4x4",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IMatrix4x4 pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IMatrix4x4 pointer
		 * @return a pointer to the IMatrix4x4 object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IMatrix4x4 *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IMatrix4x4") )
				return static_cast<IMatrix4x4*>(ptr);
			return NULL;
			}
		/**
		 * Invert the matrix.
		 */
		virtual	void Invert( IMatrix4x4 *mtxin ) = 0;

		/**
		 * Sets/Gets an element 
		 */
		virtual	real GetElement( unsigned int column , unsigned int row ) = 0;

		/**
		 * Invert the matrix.
		 */
		virtual	void Invert(  ) = 0;

		/**
		 * Creates a 3D scaling in a homogeneous transformation 4x4 matrix
		 */
		virtual	void SetToScalingTransform( real sx , real sy , real sz ) = 0;

		/**
		 * Sets/Gets an element 
		 */
		virtual	void SetElement( unsigned int column , unsigned int row , real value ) = 0;

		/**
		 * Multiply in-place an array with full 4D vectors [x y z w]. 
		 */
		virtual	void Vector4ArrayMultiply( IRealArray *vecarray ) = 0;

		/**
		 * Multiply in-place an array with 3D points [x y z 1].
		 */
		virtual	void Point3ArrayMultiply( IRealArray *vecarray ) = 0;

		/**
		 * Gets the elements of the matrix
		 */
		virtual	void GetElements( real dest_param[16] ) = 0;

		/**
		 * Transpose the matrix.
		 */
		virtual	void Transpose( IMatrix4x4 *mtxin ) = 0;

		/**
		 * Multiply in-place an array with 3D direction vectors [x y z 0]. 
		 */
		virtual	void Direction3ArrayMultiply( IRealArray *vecarray ) = 0;

		/**
		 * Creates a 3D rotation in a homogeneous transformation 4x4 matrix around the specified
		 * axis.
		 */
		virtual	void SetToRotationTransform( real angle , real ax , real ay , real az ) = 0;

		/**
		 * Transpose the matrix.
		 */
		virtual	void Transpose(  ) = 0;

		/**
		 * Copies the contents of one matrix to the matrix
		 */
		virtual	void DeepCopy( IMatrix4x4 *mtxin ) = 0;

		/**
		 * Creates a 3D translation in a homogeneous 4x4 matrix. 
		 */
		virtual	void SetToTranslationTransform( real tx , real ty , real tz ) = 0;

		/**
		 * Set the matrix to the zero matrix (all zeros).
		 */
		virtual	void Zero(  ) = 0;

		/**
		 * Set the matrix to the identity matrix.
		 */
		virtual	void Identity(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IMatrix4x4
	 */
	typedef CountedPointer<IMatrix4x4> spMatrix4x4;

	class IMatrix4x4;
	class IRealArray;
	class ITransform3;

	/**
	 * ReTransform3 handles homogeneous 4x4  transforms, i.e. transformations which can
	 * be represented by  multiplying a 4x4 matrix with a homogeneous 3D coordinate.  ReTransform3
	 * can either premultiply a matrix onto the transformation, which will add a transform
	 * around the current transform, or it can postmultiply the matrix, which will add a
	 * transform inside the current transform. Postmultiply is the default mode.
	 */
	class ITransform3 : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ITransform3 is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ITransform3",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ITransform3 pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ITransform3 pointer
		 * @return a pointer to the ITransform3 object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ITransform3 *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ITransform3") )
				return static_cast<ITransform3*>(ptr);
			return NULL;
			}
		/**
		 * Get the real array that is used to store the matrix stack
		 */
		virtual	CountedPointer<IRealArray> GetStack(  ) = 0;

		/**
		 * Concatenates a translation transform to the current transform @param tx the translation
		 * vector's x component @param ty the translation vector's y component @param tz the
		 * translation vector's z component
		 */
		virtual	void AddTranslation( real tx , real ty , real tz ) = 0;

		/**
		 * Get/Set the transform concatenation mode, any added transform will be concatenated
		 * using this mode. 
		 */
		virtual	void PostMultiply(  ) = 0;

		/**
		 * Get/Set the transform concatenation mode, any added transform will be concatenated
		 * using this mode. 
		 */
		virtual	bool IsPreMultiply(  ) = 0;

		/**
		 * Get/Set the transform concatenation mode, any added transform will be concatenated
		 * using this mode. 
		 */
		virtual	void PreMultiply(  ) = 0;

		/**
		 * Concatenates a generic transform to the current transform @param transform the input
		 * transform
		 */
		virtual	void AddTransformation( ITransform3 *transform ) = 0;

		/**
		 * Pops the topmost matrix on the the matrix stack, and replaces the current transform
		 * with the popped matrix.
		 */
		virtual	void Pop(  ) = 0;

		/**
		 * Get/Set the transform concatenation mode, any added transform will be concatenated
		 * using this mode. 
		 */
		virtual	bool IsPostMultiply(  ) = 0;

		/**
		 * Get the matrix that is used to specify the transform
		 */
		virtual	CountedPointer<IMatrix4x4> GetMatrix(  ) = 0;

		/**
		 * Concatenates a rotation transform to the current transform @param angle the rotation
		 * angle around the axis @param rx the x axis rotation component @param ry the y axis
		 * rotation component @param rz the z axis rotation component
		 */
		virtual	void AddRotation( real angle , real rx , real ry , real rz ) = 0;

		/**
		 * Concatenates a scaling transform to the current transform @param sx the scaling vector's
		 * x axis component  @param sy the scaling vector's y axis component  @param sz the
		 * scaling vector's z axis component 
		 */
		virtual	void AddScaling( real sx , real sy , real sz ) = 0;

		/**
		 * Concatenates a generic transform to the current transform @param transform the input
		 * transform
		 */
		virtual	void AddTransformation( IMatrix4x4 *transform ) = 0;

		/**
		 * Pushes the current transform onto the matrix stack
		 */
		virtual	void Push(  ) = 0;

		/**
		 * Resets the transformation to the identity transform, resets the  multiplication mode
		 * to postmultiply, and clears the matrix stack
		 */
		virtual	void Clear(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ITransform3
	 */
	typedef CountedPointer<ITransform3> spTransform3;


	/**
	 * SgMappingImage stores the mapping between a processed geometry and the source geometry
	 * it is based on. The mapping image object also stores multisampling settings. It can
	 * be used to cast information from the original geometry to the processed geometry,
	 * such as surface transfer or calculating normal maps. See SgMaterialCaster and subclasses
	 * for casting uses.
	 */
	class IMappingImage : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IMappingImage is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IMappingImage",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IMappingImage pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IMappingImage pointer
		 * @return a pointer to the IMappingImage object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IMappingImage *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IMappingImage") )
				return static_cast<IMappingImage*>(ptr);
			return NULL;
			}
		/**
		 * Get the width/height of the mapping texture. 
		 */
		virtual	unsigned int GetLayerCount(  ) = 0;

		/**
		 * Get the width/height of the mapping texture. 
		 */
		virtual	unsigned int GetWidth(  ) = 0;

		/**
		 * Get the width/height of the mapping texture. 
		 */
		virtual	unsigned int GetHeight(  ) = 0;

		/**
		 * Get the multisampling width/height of the mapping texture. 
		 */
		virtual	unsigned int GetMultisamplingWidth(  ) = 0;

		/**
		 * Get the multisampling width/height of the mapping texture. 
		 */
		virtual	unsigned int GetMultisamplingHeight(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IMappingImage
	 */
	typedef CountedPointer<IMappingImage> spMappingImage;


	/**
	 * SgProcessingObject is the abstract base class for all processing objects in Simplygon.
	 * Processing objects take parameters, exports the RunProcessing to run the main processing
	 * and Clear to  clear the internal states of the processing object. All processing
	 * objects also export information on how far the process has moved and the possibility
	 * to cancel long running processings.
	 */
	class IProcessingObject : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IProcessingObject is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IProcessingObject",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IProcessingObject pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IProcessingObject pointer
		 * @return a pointer to the IProcessingObject object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IProcessingObject *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IProcessingObject") )
				return static_cast<IProcessingObject*>(ptr);
			return NULL;
			}
		/**
		 * Clear all internal states of the object. This will NOT clear the parameters set in
		 * the object.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Runs the processing the object is designed for
		 */
		virtual	void RunProcessing(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IProcessingObject
	 */
	typedef CountedPointer<IProcessingObject> spProcessingObject;

	class IGeometryData;
	class IGeometryDataCollection;

	/**
	 * a grouping of objects that process in conjunction
	 */
	class IGeometryGroup : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGeometryGroup is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGeometryGroup",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGeometryGroup pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGeometryGroup pointer
		 * @return a pointer to the IGeometryGroup object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGeometryGroup *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGeometryGroup") )
				return static_cast<IGeometryGroup*>(ptr);
			return NULL;
			}
		/**
		 * Sets the group from a geometry collection. This will clear the group, and append
		 * all geometries in the collection, in the order they are listed within the collection.
		 * Returns the number of added geometries from the collection
		 */
		virtual	unsigned int SetFromGeometryCollection( IGeometryDataCollection *collection ) = 0;

		/**
		 * Appends geometry data to the combined geometry data, the return value is the id of
		 * the object within the group. All triangles in the combined geometry that comes from
		 * this geometry will have the field "GroupId" set to this id. Please note that the
		 * original geometry will not be placed in the group, but  a copy of the object. Note!
		 * That the field "GroupId" must not exist in the  source geometry.
		 */
		virtual	rid AppendGeometryData( IGeometryData *source_geometry ) = 0;

		/**
		 * Set/Get the CopyNewCombinedFields flag. if true, the split  geometry data objects
		 * will receive newly added fields in the combined geometry data object.
		 */
		virtual	void SetCopyNewCombinedFields( bool value ) = 0;

		/**
		 * Set/Get the name of a geometry group
		 */
		virtual	void SetGroupName( rid group_id , const char * str ) = 0;

		/**
		 * Set/Get the CopyNewCombinedFields flag. if true, the split  geometry data objects
		 * will receive newly added fields in the combined geometry data object.
		 */
		virtual	bool GetCopyNewCombinedFields(  ) = 0;

		/**
		 * Retreive a pointer to a specific geometry within the group. The id is the same as
		 * was returned when appending the geometry to the group.  Before returning, the object
		 * returned will be updated from the combined geometry, this may impact preformance
		 * if the function is unnecessary called.  Please note that the returnd geometry is
		 * not the same as was appended, but a  copy of that geometry. The same geometry is
		 * however returned each time.
		 */
		virtual	CountedPointer<IGeometryData> GetSplitGeometryData( rid geometry_id ) = 0;

		/**
		 * Extracts all geometries into a collection. Please note that  just as in GetSplitGeometryData,
		 * the same geometry objects are extracted  and returned each time. Because of setup
		 * time, calling this method is  much faster than calling GetSplitGeometryData for each
		 * geometry in the group.
		 */
		virtual	CountedPointer<IGeometryDataCollection> NewSplitGeometryDataCollection(  ) = 0;

		/**
		 * Set/Get the name of a geometry group
		 */
		virtual	rstring GetGroupName( rid group_id ) = 0;

		/**
		 * Clears the group, releases all geometry data
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Sets the combined geometry and split geometries from  a geometry that already contains
		 * GroupIds. Note that the  actual geometry is now referenced, and will be returned
		 * by  a call to GetCombinedGeometry(). The combined geometry *must* have group ids
		 * in the range 0 -> (group_counts-1).
		 */
		virtual	void ConnectToExistingCombinedGeometry( IGeometryData *geom , unsigned int group_counts ) = 0;

		/**
		 * Get the combined geometry object
		 */
		virtual	CountedPointer<IGeometryData> GetCombinedGeometry(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGeometryGroup
	 */
	typedef CountedPointer<IGeometryGroup> spGeometryGroup;


	/**
	 * ReObjectCollection and its specializations handles a collection of  ReObjects. There
	 * are methods for adding, removing and iterating through  the objects.
	 */
	class IObjectCollection : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IObjectCollection is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IObjectCollection",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IObjectCollection pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IObjectCollection pointer
		 * @return a pointer to the IObjectCollection object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IObjectCollection *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IObjectCollection") )
				return static_cast<IObjectCollection*>(ptr);
			return NULL;
			}
		/**
		 * Get the number of items in the collection.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Adds an object to the collection, as AddObject, but sorts the object into the collection
		 * based on its name.
		 */
		virtual	rhandle AddObjectSorted( IObject *obj , bool ascending ) = 0;

		/**
		 * Finds a named object in the collection. The first object  with this name is returned.
		 * The search is case-sensitive.
		 */
		virtual	CountedPointer<IObject> FindObject( const char * name ) = 0;

		/**
		 * Adds an object to the collection. The handle refers to the object until the object
		 * is removed from the collection again.
		 */
		virtual	rhandle AddObject( IObject *obj ) = 0;

		/**
		 * Gets the object the handle is refering to.
		 */
		virtual	CountedPointer<IObject> GetItemsObject( rhandle objhandle ) = 0;

		/**
		 * Returns the object of the i:th item
		 */
		virtual	CountedPointer<IObject> GetItemAsObject( unsigned int index ) = 0;

		/**
		 * Returns the start of the collection. The handle returned referes to the first object
		 * in the collection. If the handle is  NULL, then the collection is empty.
		 */
		virtual	rhandle GetFirstItem(  ) = 0;

		/**
		 * Returns the handle to the next item in the collection. Use  GetFirstItem and this
		 * method to step through the collection. If the return is NULL, the end of the collection
		 * has been reached.
		 */
		virtual	rhandle GetNextItem( rhandle objhandle ) = 0;

		/**
		 * Removes an item from the collection. The handle returned is the first item after
		 * this item that is in the collection. If the return is NULL, the item was the last
		 * item in the collection. Warning! The handle is invalid after the removal of the object.
		 */
		virtual	rhandle RemoveItem( rhandle objhandle ) = 0;

		/**
		 * Returns the handle of the i:th item
		 */
		virtual	rhandle GetItem( unsigned int index ) = 0;

		/**
		 * Returns true if the object is in the collection
		 */
		virtual	bool IsObjectInCollection( IObject *pobj ) = 0;

		/**
		 * Returns true if the item is in the collection
		 */
		virtual	bool IsItemInCollection( rhandle objhandle ) = 0;

		/**
		 * Removes an object from the collection. Only the first occurance of the object is
		 * removed from the collection. 
		 */
		virtual	void RemoveObject( IObject *pobj ) = 0;

		/**
		 * Removes all items from the collection.
		 */
		virtual	void RemoveAllItems(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IObjectCollection
	 */
	typedef CountedPointer<IObjectCollection> spObjectCollection;

	class IGeometryData;

	/**
	 * ReGeometryDataCollection holds a number of ReGeometryData  objects. The objects can
	 * be looked up by their names (if they have  been named through ReObject::SetName()
	 * ).
	 */
	class IGeometryDataCollection : public IObjectCollection
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGeometryDataCollection is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGeometryDataCollection",type)==0 )
				return true;
			return IObjectCollection::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGeometryDataCollection pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGeometryDataCollection pointer
		 * @return a pointer to the IGeometryDataCollection object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGeometryDataCollection *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGeometryDataCollection") )
				return static_cast<IGeometryDataCollection*>(ptr);
			return NULL;
			}
		/**
		 * Gets the object the handle is refering to.
		 */
		virtual	CountedPointer<IObject> GetItemsObject( rhandle objhandle ) = 0;

		/**
		 * Removes an object from the collection. Only the first occurance of the object is
		 * removed from the collection. 
		 */
		virtual	void RemoveObject( IObject *pobj ) = 0;

		/**
		 * Finds a named object in the collection. The first object  with this name is returned.
		 * The search is case-sensitive.
		 */
		virtual	CountedPointer<IObject> FindObject( const char * name ) = 0;

		/**
		 * Returns the handle of the i:th item
		 */
		virtual	rhandle GetItem( unsigned int index ) = 0;

		/**
		 * Returns true if the item is in the collection
		 */
		virtual	bool IsItemInCollection( rhandle objhandle ) = 0;

		/**
		 * Removes an item from the collection. The handle returned is the first item after
		 * this item that is in the collection. If the return is NULL, the item was the last
		 * item in the collection. Warning! The handle is invalid after the removal of the object.
		 */
		virtual	rhandle RemoveItem( rhandle objhandle ) = 0;

		/**
		 * Gets the object the handle is refering to.
		 */
		virtual	CountedPointer<IGeometryData> GetGeometryData( rhandle objhandle ) = 0;

		/**
		 * Adds an object object to the collection. The handle refers  to the object until it
		 * is removed from the collection again.
		 */
		virtual	rhandle AddGeometryData( IGeometryData *obj ) = 0;

		/**
		 * Returns the start of the collection. The handle returned referes to the first object
		 * in the collection. If the handle is  NULL, then the collection is empty.
		 */
		virtual	rhandle GetFirstItem(  ) = 0;

		/**
		 * Returns the object of the i:th item
		 */
		virtual	CountedPointer<IObject> GetItemAsObject( unsigned int index ) = 0;

		/**
		 * Removes an object from the collection. Only the first occurance of the object is
		 * removed. 
		 */
		virtual	void RemoveGeometryData( IGeometryData *pobj ) = 0;

		/**
		 * Returns the handle to the next item in the collection. Use  GetFirstItem and this
		 * method to step through the collection. If the return is NULL, the end of the collection
		 * has been reached.
		 */
		virtual	rhandle GetNextItem( rhandle objhandle ) = 0;

		/**
		 * Adds an object to the collection. The handle refers to the object until the object
		 * is removed from the collection again.
		 */
		virtual	rhandle AddObject( IObject *obj ) = 0;

		/**
		 * Removes all items from the collection.
		 */
		virtual	void RemoveAllItems(  ) = 0;

		/**
		 * Returns true if the object is in the collection
		 */
		virtual	bool IsObjectInCollection( IObject *pobj ) = 0;

		/**
		 * Finds a named object. 
		 */
		virtual	CountedPointer<IGeometryData> FindGeometryData( const char * name ) = 0;

		/**
		 * Get the number of items in the collection.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Returns the object of the next item.
		 */
		virtual	CountedPointer<IGeometryData> GetNextItemAsGeometryData( rhandle *phandle ) = 0;

		/**
		 * Returns true if the object is in the collection
		 */
		virtual	bool IsGeometryDataInCollection( IGeometryData *pobj ) = 0;

		/**
		 * Adds an object to the collection, as AddObject, but sorts the object into the collection
		 * based on its name.
		 */
		virtual	rhandle AddObjectSorted( IObject *obj , bool ascending ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGeometryDataCollection
	 */
	typedef CountedPointer<IGeometryDataCollection> spGeometryDataCollection;

	class IGraphicsImporter;
	class IMaterialTable;
	class IGeometryDataCollection;
	class IGeometryData;

	/**
	 * base class for graphics import classes
	 */
	class IGraphicsImporter : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGraphicsImporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGraphicsImporter",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGraphicsImporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGraphicsImporter pointer
		 * @return a pointer to the IGraphicsImporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGraphicsImporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGraphicsImporter") )
				return static_cast<IGraphicsImporter*>(ptr);
			return NULL;
			}
		/**
		 * Runs the import. Note that all parameters must be setup before importing.
		 */
		virtual	bool RunImport(  ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. Some importers, such
		 * as SgWavefrontImporter, also have seconday files that may be set. For info regarding
		 * these secondary files, please see the documentation for the specific importer.
		 */
		virtual	rstring GetImportFilePath(  ) = 0;

		/**
		 * Close any open file or stream, release any allocated data 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. Some importers, such
		 * as SgWavefrontImporter, also have seconday files that may be set. For info regarding
		 * these secondary files, please see the documentation for the specific importer.
		 */
		virtual	void SetImportFilePath( const char * value ) = 0;

		/**
		 * Get the material table that is used by the impoter. The importer will reference the
		 * table util Clear() is called.
		 */
		virtual	CountedPointer<IMaterialTable> GetMaterials(  ) = 0;

		/**
		 * Get the geometry data collection object that is filled with  geometries from the
		 * importer. The importer will reference the table util Clear() is called.
		 */
		virtual	CountedPointer<IGeometryDataCollection> GetGeometries(  ) = 0;

		/**
		 * Get the first geometry data in the Geometries collection. This is useful  if Geometries
		 * is known to only contain one object. 
		 */
		virtual	CountedPointer<IGeometryData> GetFirstGeometry(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGraphicsImporter
	 */
	typedef CountedPointer<IGraphicsImporter> spGraphicsImporter;

	class IGraphicsImporter;
	class IMaterialTable;
	class IGeometryDataCollection;
	class IGeometryData;

	/**
	 * base class for graphics import classes
	 */
	class IWavefrontImporter : public IGraphicsImporter
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IWavefrontImporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IWavefrontImporter",type)==0 )
				return true;
			return IGraphicsImporter::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IWavefrontImporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IWavefrontImporter pointer
		 * @return a pointer to the IWavefrontImporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IWavefrontImporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IWavefrontImporter") )
				return static_cast<IWavefrontImporter*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the main import file path. This must always be set. Some importers, such
		 * as SgWavefrontImporter, also have seconday files that may be set. For info regarding
		 * these secondary files, please see the documentation for the specific importer.
		 */
		virtual	void SetImportFilePath( const char * value ) = 0;

		/**
		 * Get the geometry data collection object that is filled with  geometries from the
		 * importer. The importer will reference the table util Clear() is called.
		 */
		virtual	CountedPointer<IGeometryDataCollection> GetGeometries(  ) = 0;

		/**
		 * ExtractGroups. If set, each group within the wavefront file is extracted into a separate
		 * geometry data object, that is named as the group. If not set, the whole file  is
		 * imported into one gometry data, and groups are ignored.
		 */
		virtual	bool GetExtractGroups(  ) = 0;

		/**
		 * Get the material table that is used by the impoter. The importer will reference the
		 * table util Clear() is called.
		 */
		virtual	CountedPointer<IMaterialTable> GetMaterials(  ) = 0;

		/**
		 * ExtractGroups. If set, each group within the wavefront file is extracted into a separate
		 * geometry data object, that is named as the group. If not set, the whole file  is
		 * imported into one gometry data, and groups are ignored.
		 */
		virtual	void SetExtractGroups( bool value ) = 0;

		/**
		 * Runs the import. Note that all parameters must be setup before importing.
		 */
		virtual	bool RunImport(  ) = 0;

		/**
		 * Set/Get the material file path. If the path is not set, it is assumed to equal the
		 * path of the main obj file, but with the extension '.mtl' instead of '.obj'. If no
		 * material file is found or specified, materials that are found in the Wavefront file
		 * is added using default material settings to the material table.
		 */
		virtual	void SetMaterialFilePath( const char * value ) = 0;

		/**
		 * Set/Get the material file path. If the path is not set, it is assumed to equal the
		 * path of the main obj file, but with the extension '.mtl' instead of '.obj'. If no
		 * material file is found or specified, materials that are found in the Wavefront file
		 * is added using default material settings to the material table.
		 */
		virtual	rstring GetMaterialFilePath(  ) = 0;

		/**
		 * Get the first geometry data in the Geometries collection. This is useful  if Geometries
		 * is known to only contain one object. 
		 */
		virtual	CountedPointer<IGeometryData> GetFirstGeometry(  ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. Some importers, such
		 * as SgWavefrontImporter, also have seconday files that may be set. For info regarding
		 * these secondary files, please see the documentation for the specific importer.
		 */
		virtual	rstring GetImportFilePath(  ) = 0;

		/**
		 * Close any open file or stream, release any allocated data 
		 */
		virtual	void Clear(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IWavefrontImporter
	 */
	typedef CountedPointer<IWavefrontImporter> spWavefrontImporter;

	class IMaterialTable;
	class IGeometryDataCollection;
	class IGeometryData;

	/**
	 * base class for graphics export classes
	 */
	class IGraphicsExporter : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGraphicsExporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGraphicsExporter",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGraphicsExporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGraphicsExporter pointer
		 * @return a pointer to the IGraphicsExporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGraphicsExporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGraphicsExporter") )
				return static_cast<IGraphicsExporter*>(ptr);
			return NULL;
			}
		/**
		 * Tells the exporter that only one geometry is to be exported. The exporter will allocate
		 * a GeometryDataCollection object, and add the geometry into it.  @param geom The one
		 * geometry that will be exported.
		 */
		virtual	void SetSingleGeometry( IGeometryData *geom ) = 0;

		/**
		 * Set/Get the main export file path. This main file being written to, and must always
		 * be set. 
		 */
		virtual	void SetExportFilePath( const char * value ) = 0;

		/**
		 * Set the material table that is to be written. 
		 */
		virtual	void SetMaterials( IMaterialTable *value ) = 0;

		/**
		 * Set/Get the main export file path. This main file being written to, and must always
		 * be set. 
		 */
		virtual	rstring GetExportFilePath(  ) = 0;

		/**
		 * Runs the export. Note that all required parameters must be setup before exporting.
		 */
		virtual	bool RunExport(  ) = 0;

		/**
		 * Set the geometry data collection object that is to be written
		 */
		virtual	void SetGeometries( IGeometryDataCollection *value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGraphicsExporter
	 */
	typedef CountedPointer<IGraphicsExporter> spGraphicsExporter;

	class IMaterialTable;
	class IGeometryDataCollection;
	class IGeometryData;

	/**
	 * Wavefront .obj geometry exporter
	 */
	class IWavefrontExporter : public IGraphicsExporter
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IWavefrontExporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IWavefrontExporter",type)==0 )
				return true;
			return IGraphicsExporter::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IWavefrontExporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IWavefrontExporter pointer
		 * @return a pointer to the IWavefrontExporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IWavefrontExporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IWavefrontExporter") )
				return static_cast<IWavefrontExporter*>(ptr);
			return NULL;
			}
		/**
		 * Set the material table that is to be written. 
		 */
		virtual	void SetMaterials( IMaterialTable *value ) = 0;

		/**
		 * Tells the exporter that only one geometry is to be exported. The exporter will allocate
		 * a GeometryDataCollection object, and add the geometry into it.  Specific for WavefrontExporter:
		 * If the geometry contains GroupIds, these are used and the wavefront file will contain
		 * g statements with names in the  form: group_id_XX where XX is the id in the GroupIds
		 * array  @param geom The one geometry that will be exported.
		 */
		virtual	void SetSingleGeometry( IGeometryData *geom ) = 0;

		/**
		 * Set the geometry data collection object that is to be written
		 */
		virtual	void SetGeometries( IGeometryDataCollection *value ) = 0;

		/**
		 * Set/Get the main export file path. This main file being written to, and must always
		 * be set. 
		 */
		virtual	rstring GetExportFilePath(  ) = 0;

		/**
		 * Set/Get the main export file path. This main file being written to, and must always
		 * be set. 
		 */
		virtual	void SetExportFilePath( const char * value ) = 0;

		/**
		 * Set/Get the material file path. If the path is not set, it is assumed to equal the
		 * path of the main obj file, but with the extension '.mtl' instead of '.obj'. 
		 */
		virtual	rstring GetMaterialFilePath(  ) = 0;

		/**
		 * Runs the export. Note that all required parameters must be setup before exporting.
		 */
		virtual	bool RunExport(  ) = 0;

		/**
		 * Set/Get the material file path. If the path is not set, it is assumed to equal the
		 * path of the main obj file, but with the extension '.mtl' instead of '.obj'. 
		 */
		virtual	void SetMaterialFilePath( const char * value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IWavefrontExporter
	 */
	typedef CountedPointer<IWavefrontExporter> spWavefrontExporter;

	class IGraphicsImporter;
	class IMaterialTable;
	class IGeometryDataCollection;
	class IGeometryData;

	/**
	 * SgBinaryImporter loads the geometry data from a binary file, that is specific for
	 * Simplygon. Please note that the binary file is only intended for temporary storage,
	 * and that no compatibility between versions of Simplygon is guaranteed.
	 */
	class IBinaryImporter : public IGraphicsImporter
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IBinaryImporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IBinaryImporter",type)==0 )
				return true;
			return IGraphicsImporter::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IBinaryImporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IBinaryImporter pointer
		 * @return a pointer to the IBinaryImporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IBinaryImporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IBinaryImporter") )
				return static_cast<IBinaryImporter*>(ptr);
			return NULL;
			}
		/**
		 * Get the first geometry data in the Geometries collection. This is useful  if Geometries
		 * is known to only contain one object. 
		 */
		virtual	CountedPointer<IGeometryData> GetFirstGeometry(  ) = 0;

		/**
		 * Runs the import. Note that all parameters must be setup before importing.
		 */
		virtual	bool RunImport(  ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. Some importers, such
		 * as SgWavefrontImporter, also have seconday files that may be set. For info regarding
		 * these secondary files, please see the documentation for the specific importer.
		 */
		virtual	rstring GetImportFilePath(  ) = 0;

		/**
		 * Close any open file or stream, release any allocated data 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Get the material table that is used by the impoter. The importer will reference the
		 * table util Clear() is called.
		 */
		virtual	CountedPointer<IMaterialTable> GetMaterials(  ) = 0;

		/**
		 * Get the geometry data collection object that is filled with  geometries from the
		 * importer. The importer will reference the table util Clear() is called.
		 */
		virtual	CountedPointer<IGeometryDataCollection> GetGeometries(  ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. Some importers, such
		 * as SgWavefrontImporter, also have seconday files that may be set. For info regarding
		 * these secondary files, please see the documentation for the specific importer.
		 */
		virtual	void SetImportFilePath( const char * value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IBinaryImporter
	 */
	typedef CountedPointer<IBinaryImporter> spBinaryImporter;

	class IMaterialTable;
	class IGeometryDataCollection;
	class IGeometryData;

	/**
	 * SgBinaryExporter stores the geometry data into a binary file, that is specific for
	 * Simplygon. Please note that the binary file is only intended for temporary storage,
	 * and that no compatibility between versions of Simplygon is guaranteed.
	 */
	class IBinaryExporter : public IGraphicsExporter
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IBinaryExporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IBinaryExporter",type)==0 )
				return true;
			return IGraphicsExporter::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IBinaryExporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IBinaryExporter pointer
		 * @return a pointer to the IBinaryExporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IBinaryExporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IBinaryExporter") )
				return static_cast<IBinaryExporter*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the main export file path. This main file being written to, and must always
		 * be set. 
		 */
		virtual	void SetExportFilePath( const char * value ) = 0;

		/**
		 * Set the material table that is to be written. 
		 */
		virtual	void SetMaterials( IMaterialTable *value ) = 0;

		/**
		 * Tells the exporter that only one geometry is to be exported. The exporter will allocate
		 * a GeometryDataCollection object, and add the geometry into it.  @param geom The one
		 * geometry that will be exported.
		 */
		virtual	void SetSingleGeometry( IGeometryData *geom ) = 0;

		/**
		 * Set the geometry data collection object that is to be written
		 */
		virtual	void SetGeometries( IGeometryDataCollection *value ) = 0;

		/**
		 * Runs the export. Note that all required parameters must be setup before exporting.
		 */
		virtual	bool RunExport(  ) = 0;

		/**
		 * Set/Get the main export file path. This main file being written to, and must always
		 * be set. 
		 */
		virtual	rstring GetExportFilePath(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IBinaryExporter
	 */
	typedef CountedPointer<IBinaryExporter> spBinaryExporter;

	class IGeometryData;

	/**
	 * checks the geometry for consistant indices, number of tringles etc
	 */
	class IGeometryValidator : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGeometryValidator is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGeometryValidator",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGeometryValidator pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGeometryValidator pointer
		 * @return a pointer to the IGeometryValidator object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGeometryValidator *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGeometryValidator") )
				return static_cast<IGeometryValidator*>(ptr);
			return NULL;
			}
		/**
		 * Returns a string describing the error that was found for the last check that was
		 * performed.
		 */
		virtual	rstring GetErrorString(  ) = 0;

		/**
		 * Returns the error-code for the last check that was performed. 0 means no error was
		 * found, and the values less than 0 means an error was found.
		 */
		virtual	rid GetErrorValue(  ) = 0;

		/**
		 * Checks if the data stored in the Geometry is stored correctly. Will return True if
		 * the geometry is stored correct, or false if something is wrong.
		 */
		virtual	bool ValidateGeometry( IGeometryData *geom ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGeometryValidator
	 */
	typedef CountedPointer<IGeometryValidator> spGeometryValidator;

	class IMaterialTable;
	class IMappingImage;
	class IImageData;

	/**
	 * SgMaterialCaster is the base interface for material  casting interfaces. The interface
	 * exports methods for defining input geometries & mapping image, materials, output
	 * image paths & formats
	 */
	class IMaterialCaster : public IProcessingObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IMaterialCaster is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IMaterialCaster",type)==0 )
				return true;
			return IProcessingObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IMaterialCaster pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IMaterialCaster pointer
		 * @return a pointer to the IMaterialCaster object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IMaterialCaster *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IMaterialCaster") )
				return static_cast<IMaterialCaster*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	rstring GetOutputFilePath(  ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	void SetDestMaterialId( rid value ) = 0;

		/**
		 * Clears the processing, and resets all internal states. 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void CastMaterials(  ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputFilePath( const char * value ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	void SetSourceMaterials( IMaterialTable *value ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputImage( IImageData *value ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	void SetOutputChannels( unsigned int value ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	CountedPointer<IMappingImage> GetMappingImage(  ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	void SetOutputChannelBitDepth( unsigned int value ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	CountedPointer<IImageData> GetOutputImage(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	unsigned int GetOutputChannelBitDepth(  ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	CountedPointer<IMaterialTable> GetSourceMaterials(  ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	rid GetDestMaterialId(  ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	void SetMappingImage( IMappingImage *value ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	void SetDilation( unsigned int value ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	unsigned int GetDilation(  ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	unsigned int GetOutputChannels(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IMaterialCaster
	 */
	typedef CountedPointer<IMaterialCaster> spMaterialCaster;

	class IMaterialTable;
	class IMappingImage;
	class IImageData;

	/**
	 * SgColorCaster is the interface used to cast basic color values. Currently supported
	 * material color value types are Ambient,  Diffuse and Specular + Shininess.
	 */
	class IColorCaster : public IMaterialCaster
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IColorCaster is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IColorCaster",type)==0 )
				return true;
			return IMaterialCaster::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IColorCaster pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IColorCaster pointer
		 * @return a pointer to the IColorCaster object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IColorCaster *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IColorCaster") )
				return static_cast<IColorCaster*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	unsigned int GetOutputChannelBitDepth(  ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	void SetOutputChannels( unsigned int value ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	rstring GetOutputFilePath(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void CastMaterials(  ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputImage( IImageData *value ) = 0;

		/**
		 * Set/Get the sRGB flag. If set, textures are assumed to  be in sRGB format. Default
		 * true.
		 */
		virtual	bool GetsRGB(  ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	CountedPointer<IMaterialTable> GetSourceMaterials(  ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	void SetDestMaterialId( rid value ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	CountedPointer<IImageData> GetOutputImage(  ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputFilePath( const char * value ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	rid GetDestMaterialId(  ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	void SetMappingImage( IMappingImage *value ) = 0;

		/**
		 * Clears the processing, and resets all internal states. 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	unsigned int GetOutputChannels(  ) = 0;

		/**
		 * Set/Get the color type that is cast. The valid types are Ambient, Diffuse, Specular,
		 * Emissive
		 */
		virtual	void SetColorType( const char * value ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	CountedPointer<IMappingImage> GetMappingImage(  ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	void SetDilation( unsigned int value ) = 0;

		/**
		 * Set/Get the color type that is cast. The valid types are Ambient, Diffuse, Specular,
		 * Emissive
		 */
		virtual	rstring GetColorType(  ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	void SetSourceMaterials( IMaterialTable *value ) = 0;

		/**
		 * Set/Get the sRGB flag. If set, textures are assumed to  be in sRGB format. Default
		 * true.
		 */
		virtual	void SetsRGB( bool value ) = 0;

		/**
		 * Set/Get the BakeVertexColors flag. If set, vertex colors from the original mesh will
		 * be baked into the resulting image.
		 */
		virtual	bool GetBakeVertexColors(  ) = 0;

		/**
		 * Set/Get the BakeVertexColors flag. If set, vertex colors from the original mesh will
		 * be baked into the resulting image.
		 */
		virtual	void SetBakeVertexColors( bool value ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	void SetOutputChannelBitDepth( unsigned int value ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	unsigned int GetDilation(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IColorCaster
	 */
	typedef CountedPointer<IColorCaster> spColorCaster;

	class IMaterialTable;
	class IMappingImage;
	class IImageData;

	/**
	 * SgNormalCaster is the interface used to cast normals data onto the receiving geometry.
	 */
	class INormalCaster : public IMaterialCaster
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if INormalCaster is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("INormalCaster",type)==0 )
				return true;
			return IMaterialCaster::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a INormalCaster pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a INormalCaster pointer
		 * @return a pointer to the INormalCaster object, if the cast can be made, and a NULL pointer otherwise
		 */
		static INormalCaster *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("INormalCaster") )
				return static_cast<INormalCaster*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the texture coordinate level to use for the  tangent space normals.
		 */
		virtual	rid GetNormalMapTextureLevel(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	CountedPointer<IMaterialTable> GetSourceMaterials(  ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	void SetOutputChannels( unsigned int value ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	rid GetDestMaterialId(  ) = 0;

		/**
		 * Clears the processing, and resets all internal states. 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	CountedPointer<IImageData> GetOutputImage(  ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	unsigned int GetOutputChannels(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void CastMaterials(  ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	unsigned int GetDilation(  ) = 0;

		/**
		 * Set/Get the FlipBackfacingNormals flag. If set, then normals will be flipped if they
		 * are back facing, ie, point  into the surface. This may introduce artifacts on geometries
		 * that have correct facing normals, so only use for geometries with known back-facing
		 * normals.
		 */
		virtual	bool GetFlipBackfacingNormals(  ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	void SetOutputChannelBitDepth( unsigned int value ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	void SetDestMaterialId( rid value ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	void SetSourceMaterials( IMaterialTable *value ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	void SetMappingImage( IMappingImage *value ) = 0;

		/**
		 * Set/Get the texture coordinate level to use for the  tangent space normals.
		 */
		virtual	void SetNormalMapTextureLevel( rid value ) = 0;

		/**
		 * Set/Get the GenerateTangentSpaceNormals flag. If set, then the generated normal map
		 * will be in tangent space. Please note that the geometry casted to must contain tangent
		 * space fields.
		 */
		virtual	void SetGenerateTangentSpaceNormals( bool value ) = 0;

		/**
		 * Set/Get the GenerateTangentSpaceNormals flag. If set, then the generated normal map
		 * will be in tangent space. Please note that the geometry casted to must contain tangent
		 * space fields.
		 */
		virtual	bool GetGenerateTangentSpaceNormals(  ) = 0;

		/**
		 * Set/Get the FlipBackfacingNormals flag. If set, then normals will be flipped if they
		 * are back facing, ie, point  into the surface. This may introduce artifacts on geometries
		 * that have correct facing normals, so only use for geometries with known back-facing
		 * normals.
		 */
		virtual	void SetFlipBackfacingNormals( bool value ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	unsigned int GetOutputChannelBitDepth(  ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	CountedPointer<IMappingImage> GetMappingImage(  ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	void SetDilation( unsigned int value ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputFilePath( const char * value ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	rstring GetOutputFilePath(  ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputImage( IImageData *value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an INormalCaster
	 */
	typedef CountedPointer<INormalCaster> spNormalCaster;

	class IGeometryData;

	/**
	 * repairs normals on a processed geometry
	 */
	class INormalRepairer : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if INormalRepairer is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("INormalRepairer",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a INormalRepairer pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a INormalRepairer pointer
		 * @return a pointer to the INormalRepairer object, if the cast can be made, and a NULL pointer otherwise
		 */
		static INormalRepairer *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("INormalRepairer") )
				return static_cast<INormalRepairer*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the Geometry object. 
		 */
		virtual	CountedPointer<IGeometryData> GetGeometry(  ) = 0;

		/**
		 * Set/Get the ScaleByAngle flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the angle of the corner at the vertex.
		 */
		virtual	void SetScaleByAngle( bool value ) = 0;

		/**
		 * Set/Get the RepairOnlyInvalidNormals flag If set, then only those normals that are
		 * invalid  (backfacing, zero length etc) will be fixed
		 */
		virtual	void SetRepairOnlyInvalidNormals( bool value ) = 0;

		/**
		 * Repairs the normals of the geometry.  If the geometry has a "ShadingGroupIds" triangle
		 * user field, it is used to calculate new normals.
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Set/Get the Geometry object. 
		 */
		virtual	void SetGeometry( IGeometryData *value ) = 0;

		/**
		 * Set/Get the RepairOnlyInvalidNormals flag If set, then only those normals that are
		 * invalid  (backfacing, zero length etc) will be fixed
		 */
		virtual	bool GetRepairOnlyInvalidNormals(  ) = 0;

		/**
		 * Set/Get the angle, above which, is considered "hard" Note! The angle is in radians
		 */
		virtual	real GetHardEdgeAngle(  ) = 0;

		/**
		 * Set/Get the ScaleByArea flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the area of the triangle.
		 */
		virtual	void SetScaleByArea( bool value ) = 0;

		/**
		 * Set/Get the angle, above which, is considered "hard" Note! The angle is in radians
		 */
		virtual	void SetHardEdgeAngle( real value ) = 0;

		/**
		 * Set/Get the use borders flag mask
		 */
		virtual	void SetBorderFlagsMask( unsigned int value ) = 0;

		/**
		 * Set/Get the use borders flag mask
		 */
		virtual	unsigned int GetBorderFlagsMask(  ) = 0;

		/**
		 * Set/Get the ScaleByArea flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the area of the triangle.
		 */
		virtual	bool GetScaleByArea(  ) = 0;

		/**
		 * Set/Get the ScaleByAngle flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the angle of the corner at the vertex.
		 */
		virtual	bool GetScaleByAngle(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an INormalRepairer
	 */
	typedef CountedPointer<INormalRepairer> spNormalRepairer;

	class IMaterialTable;
	class IMappingImage;
	class IImageData;

	/**
	 * SgDisplacementCaster is used to store the delta-values between the original and processed
	 * geometries. The values are divided by a scaling value, and clamped into the -1 ->
	 * 1 range before being  stored into an image.
	 */
	class IDisplacementCaster : public IMaterialCaster
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IDisplacementCaster is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IDisplacementCaster",type)==0 )
				return true;
			return IMaterialCaster::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IDisplacementCaster pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IDisplacementCaster pointer
		 * @return a pointer to the IDisplacementCaster object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IDisplacementCaster *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IDisplacementCaster") )
				return static_cast<IDisplacementCaster*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the DistanceScaling value. All the delta values are divided by this value
		 * before storing them into an image.
		 */
		virtual	void SetDistanceScaling( real value ) = 0;

		/**
		 * Set/Get the texture coordinate level to use for the  tangent space displacement vectors.
		 */
		virtual	rid GetNormalMapTextureLevel(  ) = 0;

		/**
		 * Set/Get the DistanceScaling value. All the delta values are divided by this value
		 * before storing them into an image.
		 */
		virtual	real GetDistanceScaling(  ) = 0;

		/**
		 * Set/Get the GenerateScalarDisplacement value. If set, the displacement vectors' lengths
		 * are stored into the displacement map, not the delta values
		 */
		virtual	void SetGenerateScalarDisplacement( bool value ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	void SetOutputChannelBitDepth( unsigned int value ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	unsigned int GetDilation(  ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	void SetDestMaterialId( rid value ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	CountedPointer<IMaterialTable> GetSourceMaterials(  ) = 0;

		/**
		 * Set/Get the DestMaterialId object. If set, only the parts of the destination map
		 * that. To disable, set to -1.
		 */
		virtual	rid GetDestMaterialId(  ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	rstring GetOutputFilePath(  ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputImage( IImageData *value ) = 0;

		/**
		 * Set/Get the GenerateScalarDisplacement value. If set, the displacement vectors' lengths
		 * are stored into the displacement map, not the delta values
		 */
		virtual	bool GetGenerateScalarDisplacement(  ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	CountedPointer<IMappingImage> GetMappingImage(  ) = 0;

		/**
		 * Set/Get the Dilation value. Where applicable, such as colors and normals, the caster
		 * will fill empty pixels surrounding filled pixels with values mixed from the filled
		 * ones. This setting sets how many  pixels to fill outside the original filled pixels.
		 */
		virtual	void SetDilation( unsigned int value ) = 0;

		/**
		 * Clears the processing, and resets all internal states. 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Set/Get the MappingImage object. The MappingImage object contains the mapping between
		 * the Geometry and SourceGeometry objects.
		 */
		virtual	void SetMappingImage( IMappingImage *value ) = 0;

		/**
		 * Runs the material casting from SourceGeometry to Geometry. 
		 */
		virtual	void CastMaterials(  ) = 0;

		/**
		 * Set/Get the GenerateTangentSpaceDisplacement value. If set, the displacement vectors
		 * are transformed into the destination objects' tangent space
		 */
		virtual	void SetGenerateTangentSpaceDisplacement( bool value ) = 0;

		/**
		 * Set/Get the GenerateTangentSpaceDisplacement value. If set, the displacement vectors
		 * are transformed into the destination objects' tangent space
		 */
		virtual	bool GetGenerateTangentSpaceDisplacement(  ) = 0;

		/**
		 * Set/Get the texture coordinate level to use for the  tangent space displacement vectors.
		 */
		virtual	void SetNormalMapTextureLevel( rid value ) = 0;

		/**
		 * Set/Get the SourceMaterials object. The SourceMaterials object contains all materials
		 * of the the source geometry. The source geometry must have a "MaterialIds" field that
		 * indices the material table.
		 */
		virtual	void SetSourceMaterials( IMaterialTable *value ) = 0;

		/**
		 * Set/Get the OutputBitDepth parameter. This can be either 8 or 16.  Please note that
		 * this is the bit depth per-channel, so a bit depth of 8 and a OutputChannels value
		 * of 3 will result in 8*3 = 24-bit RGB values.
		 */
		virtual	unsigned int GetOutputChannelBitDepth(  ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	void SetOutputChannels( unsigned int value ) = 0;

		/**
		 * Set/Get the OutputChannels parameter. This can be set to either 1 (Luminance), 3
		 * (RGB) or 4 (RGBA).
		 */
		virtual	unsigned int GetOutputChannels(  ) = 0;

		/**
		 * Set/Get the OutputFilePath file path, where the output image will be placed. Note
		 * that the extension specified will specify the file format. Please note that only
		 * PNG (extension .png) will be able  to support all possible bit dephts (such as RGB
		 * 16-bit per channel). Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	void SetOutputFilePath( const char * value ) = 0;

		/**
		 * Set/Get the OutputImage object that will receive the image. The current contents
		 * of the image will be removed, and the image will be written to the Colors field of
		 * the ReImageData object. Either OutputImage or OutputFilePath must be set. If OutputImage
		 * is set, then OutputFilePath is ignored.
		 */
		virtual	CountedPointer<IImageData> GetOutputImage(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IDisplacementCaster
	 */
	typedef CountedPointer<IDisplacementCaster> spDisplacementCaster;

	class IGeometryData;

	/**
	 * ReGeometryTangentCalculator calculates tangent vectors for geometry data objects.
	 * One set of texture coodinates must exist, as well as normals. The Tangents are placed
	 * into two TriangleVertex-fields called "Tangents" and "Bitangents". If any of theese
	 * fields already exists, it is replaced by the new field.
	 */
	class IGeometryTangentCalculator : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IGeometryTangentCalculator is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IGeometryTangentCalculator",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IGeometryTangentCalculator pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IGeometryTangentCalculator pointer
		 * @return a pointer to the IGeometryTangentCalculator object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IGeometryTangentCalculator *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IGeometryTangentCalculator") )
				return static_cast<IGeometryTangentCalculator*>(ptr);
			return NULL;
			}
		/**
		 * Calculates the tangents of a geometry data object. The geometry object geom  receives
		 * new fields, as described in the class description.
		 */
		virtual	void CalculateTangents( IGeometryData *geom ) = 0;

		/**
		 * The TexCoords set id to use for the tangent calculation If set to -1, all texture
		 * coordinates will get corresponding tangent fields.
		 */
		virtual	void SetTexCoordsSetId( rid value ) = 0;

		/**
		 * Gets/Sets the tangent calculator type
		 */
		virtual	void SetTangentCalculatorType( rid type ) = 0;

		/**
		 * Gets/Sets the tangent calculator type
		 */
		virtual	rid GetTangentCalculatorType(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IGeometryTangentCalculator
	 */
	typedef CountedPointer<IGeometryTangentCalculator> spGeometryTangentCalculator;


	/**
	 * SgSettingsObject is the abstract base class for all settings objects in Simplygon.
	 * Settings objects contains parameters for processing objects to use. These settings
	 * objects will have their data set and reached from a SgProcessingObject and then used
	 * therein.
	 */
	class ISettingsObject : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISettingsObject is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISettingsObject",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISettingsObject pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISettingsObject pointer
		 * @return a pointer to the ISettingsObject object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISettingsObject *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISettingsObject") )
				return static_cast<ISettingsObject*>(ptr);
			return NULL;
			}
	};

	/**
	 * a CounterPointer smart pointer to an ISettingsObject
	 */
	typedef CountedPointer<ISettingsObject> spSettingsObject;


	/**
	 * Settings for the Simplygon repair classes
	 */
	class IRepairSettings : public ISettingsObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRepairSettings is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRepairSettings",type)==0 )
				return true;
			return ISettingsObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRepairSettings pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRepairSettings pointer
		 * @return a pointer to the IRepairSettings object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRepairSettings *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRepairSettings") )
				return static_cast<IRepairSettings*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the T-Junction distance, below which, the T-Junctions will be welded
		 */
		virtual	void SetTjuncDist( real value ) = 0;

		/**
		 * Set/Get the RemoveDoubleTriangles flag. if set, all extra triangles, containing the
		 * same three vertices as another triangle in the geometry will be removed.
		 */
		virtual	bool GetRemoveDoubleTriangles(  ) = 0;

		/**
		 * Set/Get the Welding distance, below which, the vertices will be welded
		 */
		virtual	void SetWeldDist( real value ) = 0;

		/**
		 * Set/Get the T-Junction distance, below which, the T-Junctions will be welded
		 */
		virtual	real GetTjuncDist(  ) = 0;

		/**
		 * Set/Get the Welding distance, below which, the vertices will be welded
		 */
		virtual	real GetWeldDist(  ) = 0;

		/**
		 * Set/Get the RemoveDoubleTriangles flag. if set, all extra triangles, containing the
		 * same three vertices as another triangle in the geometry will be removed.
		 */
		virtual	void SetRemoveDoubleTriangles( bool value ) = 0;

		/**
		 * Set/Get the OnlyBorder flag, if set, only veritces that are on the border (as reported
		 * by the VertexBorder boolean field) are considered for the welding
		 */
		virtual	void SetWeldOnlyBorderVertices( bool value ) = 0;

		/**
		 * Set/Get the number of ProgressivePasses. Minimum is 1, but higher numbers give a
		 * better quality, at the expense of longer runningtime.
		 */
		virtual	unsigned int GetProgressivePasses(  ) = 0;

		/**
		 * Set/Get the OnlyBorder flag, if set, only veritces that are on the border (as reported
		 * by the VertexBorder boolean field) are considered for the welding
		 */
		virtual	bool GetWeldOnlyBorderVertices(  ) = 0;

		/**
		 * Set/Get the number of ProgressivePasses. Minimum is 1, but higher numbers give a
		 * better quality, at the expense of longer runningtime.
		 */
		virtual	void SetProgressivePasses( unsigned int value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRepairSettings
	 */
	typedef CountedPointer<IRepairSettings> spRepairSettings;


	/**
	 * Settings for the Simplygon repair classes
	 */
	class IBoneSettings : public ISettingsObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IBoneSettings is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IBoneSettings",type)==0 )
				return true;
			return ISettingsObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IBoneSettings pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IBoneSettings pointer
		 * @return a pointer to the IBoneSettings object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IBoneSettings *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IBoneSettings") )
				return static_cast<IBoneSettings*>(ptr);
			return NULL;
			}
		/**
		 * Decides what kind of bone-removal that will be used (if any).
		 */
		virtual	void SetBoneLodProcess( unsigned int value ) = 0;

		/**
		 * This is the PixelSize that will be used to calculate how many bones can be removed.
		 * As many bones as possible will be removed, as long as the removal wont be noticable
		 * at the set pixelsize on screen. 
		 */
		virtual	unsigned int GetBoneLodPixelSize(  ) = 0;

		/**
		 * Decides what kind of bone-removal that will be used (if any).
		 */
		virtual	unsigned int GetBoneLodProcess(  ) = 0;

		/**
		 * Decides the maximum limit of how many bones can be connected to each vertex. If for
		 * example set to 1, then each vertex will only be linked to 1 bone.
		 */
		virtual	void SetMaxBonePerVertex( unsigned int value ) = 0;

		/**
		 * Decides the maximum limit of how many bones can be connected to each vertex. If for
		 * example set to 1, then each vertex will only be linked to 1 bone.
		 */
		virtual	unsigned int GetMaxBonePerVertex(  ) = 0;

		/**
		 * A value between 0 and 1. Decides how many bones will be removed (or rather, un-linked)
		 * from the geometry. 1 means all bones will be kept, 0.5 would mean that 50% of the
		 * bones would  be removed, and 0.1 would mean that only 10% of the bones would be kept.
		 * Leaf-bones will be removed first, and then  bones further in will follow. Vertices
		 * will be re-linked to the most suitable remaining bone.
		 */
		virtual	real GetBoneLodRatio(  ) = 0;

		/**
		 * A value between 0 and 1. Decides how many bones will be removed (or rather, un-linked)
		 * from the geometry. 1 means all bones will be kept, 0.5 would mean that 50% of the
		 * bones would  be removed, and 0.1 would mean that only 10% of the bones would be kept.
		 * Leaf-bones will be removed first, and then  bones further in will follow. Vertices
		 * will be re-linked to the most suitable remaining bone.
		 */
		virtual	void SetBoneLodRatio( real value ) = 0;

		/**
		 * This is the PixelSize that will be used to calculate how many bones can be removed.
		 * As many bones as possible will be removed, as long as the removal wont be noticable
		 * at the set pixelsize on screen. 
		 */
		virtual	void SetBoneLodPixelSize( unsigned int value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IBoneSettings
	 */
	typedef CountedPointer<IBoneSettings> spBoneSettings;


	/**
	 * Settings for the SgReducer class.
	 */
	class IReductionSettings : public ISettingsObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IReductionSettings is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IReductionSettings",type)==0 )
				return true;
			return ISettingsObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IReductionSettings pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IReductionSettings pointer
		 * @return a pointer to the IReductionSettings object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IReductionSettings *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IReductionSettings") )
				return static_cast<IReductionSettings*>(ptr);
			return NULL;
			}
		/**
		 * Flag that decides how far the reducer will go, and what settings it uses. SG_STOPCONDITION_REDUCTIONRATIO
		 * -> it will stop when the triangle-ratio has been reached. SG_STOPCONDITION_MAXDEVIATION
		 * -> it will stop when the maximum surface-deviation has been reached. SG_STOPCONDITION_BOTH_ARE_REACHED
		 * -> it will keep going until both reduction-ratio AND max-deviation has been reached.
		 * This means that it will give you the lowest trianglecount of either reduction-ratio
		 * or max-deviation. SG_STOPCONDITION_EITHER_IS_REACHED -> it will stop once either
		 * reduction-ratio OR max-deviation has been reached. This means that it will give you
		 * the highest  trianglecount of reduction-ratio and max-deviation.
		 */
		virtual	void SetStopCondition( unsigned int value ) = 0;

		/**
		 * Set/Get the EnablePostprocessing flag. 
		 */
		virtual	bool GetEnablePostprocessing(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetMaterialImportance(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetShadingImportance(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetVertexColorImportance( real value ) = 0;

		/**
		 * Set/Get the wanted reduction ratio. The range is 0->1, the number of  triangles in
		 * the processed geometry, divided by the number in the original geometry.  A value
		 * of 1 will leave all triangles in the geometry, 0 will remove as many triangles as
		 * possible. The reduction ratio may not be reachable, if other restrictions  are in
		 * place, such as a MaxDeviation. It depends on the StopCondition.
		 */
		virtual	void SetReductionRatio( real value ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetGroupImportance(  ) = 0;

		/**
		 * Set/Get the SymmetryDetectionTolerance, the tolerance used when detecting symmetry.
		 * Values in the ranges 1e-5 to 1e-3 will produce good results.
		 */
		virtual	real GetSymmetryDetectionTolerance(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetVertexColorImportance(  ) = 0;

		/**
		 * Flag that decides how far the reducer will go, and what settings it uses. SG_STOPCONDITION_REDUCTIONRATIO
		 * -> it will stop when the triangle-ratio has been reached. SG_STOPCONDITION_MAXDEVIATION
		 * -> it will stop when the maximum surface-deviation has been reached. SG_STOPCONDITION_BOTH_ARE_REACHED
		 * -> it will keep going until both reduction-ratio AND max-deviation has been reached.
		 * This means that it will give you the lowest trianglecount of either reduction-ratio
		 * or max-deviation. SG_STOPCONDITION_EITHER_IS_REACHED -> it will stop once either
		 * reduction-ratio OR max-deviation has been reached. This means that it will give you
		 * the highest  trianglecount of reduction-ratio and max-deviation.
		 */
		virtual	unsigned int GetStopCondition(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetValidityImportance( real value ) = 0;

		/**
		 * Set/Get what axis to be used for symmetry calculations. 0 = x, 1 = y, 2 = z.
		 */
		virtual	unsigned int GetSymmetryAxis(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetGroupImportance( real value ) = 0;

		/**
		 * Set/Get the AllowDegenerateTexCoords flag. If true, texture coordinates are allowed
		 * to become degenerate (triangles can have 0 area in texture space).  This may sometimes
		 * look better, but if tangent space normals are used, tangent spaces may become degenerate.
		 */
		virtual	void SetAllowDegenerateTexCoords( bool value ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetValidityImportance(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetSkinningImportance( real value ) = 0;

		/**
		 * Set/Get the MaxDeviation value, the maximum surface-deviation between the  reduced
		 * geometry and the original.
		 */
		virtual	void SetMaxDeviation( real value ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetMaterialImportance( real value ) = 0;

		/**
		 * Set/Get the EnablePreprocessing flag. 
		 */
		virtual	bool GetEnablePreprocessing(  ) = 0;

		/**
		 * Set/Get SymmetryOffset, the position on the symmetry axis where the symmetry plane
		 * is placed.
		 */
		virtual	void SetSymmetryOffset( real value ) = 0;

		/**
		 * Set/Get the flag for if the reduction process should store geomorph data in the processed
		 * GeometryData object. This data is an array with same size as the vertexcount was
		 * before the reduction process, and will have the coordinates for each vertex on its
		 * corresponding position on the reduced GeometryData's surface. Can be used to give
		 * smooth transition from a highlevel LOD to a lowlevel LOD. This array can be accessed
		 * with " GeomObj->GetCustomField( "MappedVertexCoords" ) "
		 */
		virtual	void SetGenerateGeomorphData( bool value ) = 0;

		/**
		 * Set/Get the wanted reduction ratio. The range is 0->1, the number of  triangles in
		 * the processed geometry, divided by the number in the original geometry.  A value
		 * of 1 will leave all triangles in the geometry, 0 will remove as many triangles as
		 * possible. The reduction ratio may not be reachable, if other restrictions  are in
		 * place, such as a MaxDeviation. It depends on the StopCondition.
		 */
		virtual	real GetReductionRatio(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetSkinningImportance(  ) = 0;

		/**
		 * Set/Get the AllowDegenerateTexCoords flag. If true, texture coordinates are allowed
		 * to become degenerate (triangles can have 0 area in texture space).  This may sometimes
		 * look better, but if tangent space normals are used, tangent spaces may become degenerate.
		 */
		virtual	bool GetAllowDegenerateTexCoords(  ) = 0;

		/**
		 * Set/Get the UseVertexWeighting value. If set, the reducer will weight the reduction
		 * based on the weights in the "VertexWeighting" vertex field.
		 */
		virtual	void SetUseVertexWeights( bool value ) = 0;

		/**
		 * Set/Get the MaxDeviation value, the maximum surface-deviation between the  reduced
		 * geometry and the original.
		 */
		virtual	real GetMaxDeviation(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetTextureImportance( real value ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetShadingImportance( real value ) = 0;

		/**
		 * Set/Get the UseVertexWeighting value. If set, the reducer will weight the reduction
		 * based on the weights in the "VertexWeighting" vertex field.
		 */
		virtual	bool GetUseVertexWeights(  ) = 0;

		/**
		 * Set/Get the KeepSymmetry flag. If set, the reducer will detect symmetric features,
		 * and retain symmetry during processing. Please note that for a geometry to be considered
		 * symmetrical, also the texture coordinates must be mirrored.
		 */
		virtual	void SetKeepSymmetry( bool value ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetTextureImportance(  ) = 0;

		/**
		 * Set/Get the KeepSymmetry flag. If set, the reducer will detect symmetric features,
		 * and retain symmetry during processing. Please note that for a geometry to be considered
		 * symmetrical, also the texture coordinates must be mirrored.
		 */
		virtual	bool GetKeepSymmetry(  ) = 0;

		/**
		 * Set/Get the EnablePreprocessing flag. 
		 */
		virtual	void SetEnablePreprocessing( bool value ) = 0;

		/**
		 * Set/Get what axis to be used for symmetry calculations. 0 = x, 1 = y, 2 = z.
		 */
		virtual	void SetSymmetryAxis( unsigned int value ) = 0;

		/**
		 * Set/Get SymmetryOffset, the position on the symmetry axis where the symmetry plane
		 * is placed.
		 */
		virtual	real GetSymmetryOffset(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	void SetGeometryImportance( real value ) = 0;

		/**
		 * Set/Get the SymmetryDetectionTolerance, the tolerance used when detecting symmetry.
		 * Values in the ranges 1e-5 to 1e-3 will produce good results.
		 */
		virtual	void SetSymmetryDetectionTolerance( real value ) = 0;

		/**
		 * Set/Get the EnablePostprocessing flag. 
		 */
		virtual	void SetEnablePostprocessing( bool value ) = 0;

		/**
		 * Set/Get the flag for if the reduction process should store geomorph data in the processed
		 * GeometryData object. This data is an array with same size as the vertexcount was
		 * before the reduction process, and will have the coordinates for each vertex on its
		 * corresponding position on the reduced GeometryData's surface. Can be used to give
		 * smooth transition from a highlevel LOD to a lowlevel LOD. This array can be accessed
		 * with " GeomObj->GetCustomField( "MappedVertexCoords" ) "
		 */
		virtual	bool GetGenerateGeomorphData(  ) = 0;

		/**
		 * Set/Get the importance value, the relative importance of the specific mesh feature.
		 * The importance is defined as a range 1/8 -> 8 where 1 is the default value.
		 */
		virtual	real GetGeometryImportance(  ) = 0;

		/**
		 * Set/Get the FeatureFlags mask value. Note! This parameter can only be set before
		 * starting a reduction.
		 */
		virtual	void SetFeatureFlags( unsigned int value ) = 0;

		/**
		 * Set/Get the FeatureFlags mask value. Note! This parameter can only be set before
		 * starting a reduction.
		 */
		virtual	unsigned int GetFeatureFlags(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IReductionSettings
	 */
	typedef CountedPointer<IReductionSettings> spReductionSettings;


	/**
	 * Settings for the normal recalculation 
	 */
	class INormalCalculationSettings : public ISettingsObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if INormalCalculationSettings is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("INormalCalculationSettings",type)==0 )
				return true;
			return ISettingsObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a INormalCalculationSettings pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a INormalCalculationSettings pointer
		 * @return a pointer to the INormalCalculationSettings object, if the cast can be made, and a NULL pointer otherwise
		 */
		static INormalCalculationSettings *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("INormalCalculationSettings") )
				return static_cast<INormalCalculationSettings*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the ScaleByArea flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the area of the triangle.
		 */
		virtual	bool GetScaleByArea(  ) = 0;

		/**
		 * Set/Get GenerateNormals flag. If set, it will generate new normals for the Geometry,
		 * and if not set, if will keep the normals from the original normal-set (or not add
		 * normals at all if none were present). Please note that if the original geometry contains
		 * normals, the normal repairer will replace invalid  normals even if ReplaceNormals
		 * is set to false.
		 */
		virtual	void SetReplaceNormals( bool value ) = 0;

		/**
		 * Set/Get the ScaleByArea flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the area of the triangle.
		 */
		virtual	void SetScaleByArea( bool value ) = 0;

		/**
		 * Set/Get the ScaleByAngle flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the angle of the corner at the vertex.
		 */
		virtual	bool GetScaleByAngle(  ) = 0;

		/**
		 * Set/Get the ScaleByAngle flag.  If set, the influence of a triangle normal upon the
		 * vertex normals  is scaled by the angle of the corner at the vertex.
		 */
		virtual	void SetScaleByAngle( bool value ) = 0;

		/**
		 * Set/Get HardEdgeAngle. If the angle between two triangles are above this value, the
		 * normals will not be smooth over the edge between those two triangles. Set in degrees.
		 */
		virtual	void SetHardEdgeAngle( real value ) = 0;

		/**
		 * Set/Get GenerateNormals flag. If set, it will generate new normals for the Geometry,
		 * and if not set, if will keep the normals from the original normal-set (or not add
		 * normals at all if none were present). Please note that if the original geometry contains
		 * normals, the normal repairer will replace invalid  normals even if ReplaceNormals
		 * is set to false.
		 */
		virtual	bool GetReplaceNormals(  ) = 0;

		/**
		 * Set/Get HardEdgeAngle. If the angle between two triangles are above this value, the
		 * normals will not be smooth over the edge between those two triangles. Set in degrees.
		 */
		virtual	real GetHardEdgeAngle(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an INormalCalculationSettings
	 */
	typedef CountedPointer<INormalCalculationSettings> spNormalCalculationSettings;


	/**
	 * Settings for the Simplygon mapping classes
	 */
	class IMappingImageSettings : public ISettingsObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IMappingImageSettings is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IMappingImageSettings",type)==0 )
				return true;
			return ISettingsObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IMappingImageSettings pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IMappingImageSettings pointer
		 * @return a pointer to the IMappingImageSettings object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IMappingImageSettings *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IMappingImageSettings") )
				return static_cast<IMappingImageSettings*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the texture coordinate level used for mapping image and texture generation.
		 * Default is 0.
		 */
		virtual	void SetTexCoordLevel( rid value ) = 0;

		/**
		 * Set/Get the mapping image Width value, which controls the width of the mapped texture.
		 * If set to 0, the mapping texture will be set to an appropriate size based on the
		 * OnScreenSize value.
		 */
		virtual	void SetWidth( unsigned int value ) = 0;

		/**
		 * If set, the remesher will create a mapping imagine, that maps triangles on the reduced
		 * geometry back to triangles on  the original geometry. The texture is insert into
		 * the MappingImage object. The image data will contain two fields:  * ReRidArray -
		 * "TriangleIds" - TupleSize:1 - The id of the triangle   * ReUnsignedShortArray - "BarycentricCoords"
		 * - TupleSize:2 - Items A & B of the barycentric coordinate on the triangle.  The user
		 * can control the size of the mapped texture using the values MappingTextureWidth and
		 * MappingTextureHeight
		 */
		virtual	bool GetGenerateMappingImage(  ) = 0;

		/**
		 * Generates a new texcoords field at position specified by TexCoordLevel. Will be ignored
		 * and always generate texcoords if none are available  in the Geometry, at same time
		 * as GenerateMappingImage is enabled.
		 */
		virtual	void SetGenerateTexCoords( bool value ) = 0;

		/**
		 * Set/Get the mapping image Height value, which controls the height of the mapped texture.
		 * If set to 0, the mapping texture will be set to an appropriate size based on the
		 * OnScreenSize value.
		 */
		virtual	unsigned int GetHeight(  ) = 0;

		/**
		 * The maximum allowed texture stretch. Range 0->1
		 */
		virtual	real GetMaxStretch(  ) = 0;

		/**
		 * Generates a new tangent space fields at position specified by TexCoordLevel, based
		 * on the vertex normals and texture coordinates. Tangent space fields will always be
		 * created if missing, and GenerateMappingImage is enabled. If GenerateTexCoords is
		 * enabled, new tangents will always be generated.
		 */
		virtual	bool GetGenerateTangents(  ) = 0;

		/**
		 * Set/Get the full retexturing flag. If set, all original texture sets are replaced
		 * by the one generated for the mapping image, and the TexCoordLevel value is always
		 * set to 0. This flag has no effect on the remeshing, which always does a full retexturing.
		 */
		virtual	void SetFullRetexturing( bool value ) = 0;

		/**
		 * Set/Get the multisampling level. Values 1-8 are accepted. The Number of samples are
		 * MappingTextureMultisamplingLevel^2
		 */
		virtual	void SetMultisamplingLevel( unsigned int value ) = 0;

		/**
		 * Set/Get the mapping image Width value, which controls the width of the mapped texture.
		 * If set to 0, the mapping texture will be set to an appropriate size based on the
		 * OnScreenSize value.
		 */
		virtual	unsigned int GetWidth(  ) = 0;

		/**
		 * Set/Get the texture coordinate level used for mapping image and texture generation.
		 * Default is 0.
		 */
		virtual	rid GetTexCoordLevel(  ) = 0;

		/**
		 * Generates a new texcoords field at position specified by TexCoordLevel. Will be ignored
		 * and always generate texcoords if none are available  in the Geometry, at same time
		 * as GenerateMappingImage is enabled.
		 */
		virtual	bool GetGenerateTexCoords(  ) = 0;

		/**
		 * The minimum number of pixels between charts.
		 */
		virtual	unsigned int GetGutterSpace(  ) = 0;

		/**
		 * Generates a new tangent space fields at position specified by TexCoordLevel, based
		 * on the vertex normals and texture coordinates. Tangent space fields will always be
		 * created if missing, and GenerateMappingImage is enabled. If GenerateTexCoords is
		 * enabled, new tangents will always be generated.
		 */
		virtual	void SetGenerateTangents( bool value ) = 0;

		/**
		 * Set/Get the mapping image Height value, which controls the height of the mapped texture.
		 * If set to 0, the mapping texture will be set to an appropriate size based on the
		 * OnScreenSize value.
		 */
		virtual	void SetHeight( unsigned int value ) = 0;

		/**
		 * The maximum allowed texture stretch. Range 0->1
		 */
		virtual	void SetMaxStretch( real value ) = 0;

		/**
		 * Set/Get the maximum number of layers in the mapping image. If a remeshed geometry
		 * has alot of transparent triangles, use a higher  number to be able to find intersections
		 * on the inside of parts of  the geometry. This setting only has effect in the remeshing,
		 * when  running reduction the number of layers is always 1. 
		 */
		virtual	unsigned int GetMaximumLayers(  ) = 0;

		/**
		 * The minimum number of pixels between charts.
		 */
		virtual	void SetGutterSpace( unsigned int value ) = 0;

		/**
		 * If set, the remesher will create a mapping imagine, that maps triangles on the reduced
		 * geometry back to triangles on  the original geometry. The texture is insert into
		 * the MappingImage object. The image data will contain two fields:  * ReRidArray -
		 * "TriangleIds" - TupleSize:1 - The id of the triangle   * ReUnsignedShortArray - "BarycentricCoords"
		 * - TupleSize:2 - Items A & B of the barycentric coordinate on the triangle.  The user
		 * can control the size of the mapped texture using the values MappingTextureWidth and
		 * MappingTextureHeight
		 */
		virtual	void SetGenerateMappingImage( bool value ) = 0;

		/**
		 * Set/Get the multisampling level. Values 1-8 are accepted. The Number of samples are
		 * MappingTextureMultisamplingLevel^2
		 */
		virtual	unsigned int GetMultisamplingLevel(  ) = 0;

		/**
		 * Set/Get the maximum number of layers in the mapping image. If a remeshed geometry
		 * has alot of transparent triangles, use a higher  number to be able to find intersections
		 * on the inside of parts of  the geometry. This setting only has effect in the remeshing,
		 * when  running reduction the number of layers is always 1. 
		 */
		virtual	void SetMaximumLayers( unsigned int value ) = 0;

		/**
		 * Set/Get the full retexturing flag. If set, all original texture sets are replaced
		 * by the one generated for the mapping image, and the TexCoordLevel value is always
		 * set to 0. This flag has no effect on the remeshing, which always does a full retexturing.
		 */
		virtual	bool GetFullRetexturing(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IMappingImageSettings
	 */
	typedef CountedPointer<IMappingImageSettings> spMappingImageSettings;

	class ISceneNode;
	class IGeometryData;
	class IMappingImage;
	class IRepairSettings;
	class IReductionSettings;
	class IBoneSettings;
	class INormalCalculationSettings;
	class IMappingImageSettings;

	/**
	 * SgReductionProcessor welds, removed t-junctions, removes double-triangles and reduces
	 * the trianglecount of the Geometry. It also calculates new normals for the now reduced
	 * and optimized Geometry.
	 */
	class IReductionProcessor : public IProcessingObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IReductionProcessor is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IReductionProcessor",type)==0 )
				return true;
			return IProcessingObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IReductionProcessor pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IReductionProcessor pointer
		 * @return a pointer to the IReductionProcessor object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IReductionProcessor *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IReductionProcessor") )
				return static_cast<IReductionProcessor*>(ptr);
			return NULL;
			}
		/**
		 * The settings for the repair process.
		 */
		virtual	CountedPointer<IRepairSettings> GetRepairSettings(  ) = 0;

		/**
		 * Clears the processing, and resets all internal states. If ReduceGeometry() is called
		 * after Clear(), the Geometry object is assumed to point at a new geometry object,
		 * and is reloaded.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Returns the max deviation reached for the last processing
		 */
		virtual	real GetMaxDeviation(  ) = 0;

		/**
		 * The settings for the bone removal process.
		 */
		virtual	CountedPointer<IBoneSettings> GetBoneSettings(  ) = 0;

		/**
		 * Set/Get the top scene node of a scene tree used for  reduction processing.
		 */
		virtual	void SetSceneRoot( ISceneNode *value ) = 0;

		/**
		 * Set/Get the geometry data object to reduce. If SceneRoot is set, this parameter is
		 * ignored by the reducer.
		 */
		virtual	CountedPointer<IGeometryData> GetGeometry(  ) = 0;

		/**
		 * The settings for the normal repair process.
		 */
		virtual	CountedPointer<INormalCalculationSettings> GetNormalCalculationSettings(  ) = 0;

		/**
		 * Get the MappingImage object that is generated if  MappingImageSettings::GenerateMappingImage
		 * is set. The object is of type SgMappingImage. A new object is generated for each
		 * call to RunProcessing().
		 */
		virtual	CountedPointer<IMappingImage> GetMappingImage(  ) = 0;

		/**
		 * The settings for the image mapping process.
		 */
		virtual	CountedPointer<IMappingImageSettings> GetMappingImageSettings(  ) = 0;

		/**
		 * Runs the Reductino process on the geometry. Note that GeometryInputPath OR Geometry
		 * values must be set. If Geometry is set, it will contain the remeshed geometry on
		 * return. If GeometryOutputPath is set, the geometry  will be written to the file,
		 * regardless if GeometryInputPath or Geometry is used to input the geometry data.
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Set/Get the geometry data object to reduce. If SceneRoot is set, this parameter is
		 * ignored by the reducer.
		 */
		virtual	void SetGeometry( IGeometryData *value ) = 0;

		/**
		 * Set/Get the top scene node of a scene tree used for  reduction processing.
		 */
		virtual	CountedPointer<ISceneNode> GetSceneRoot(  ) = 0;

		/**
		 * The settings for the reduction process.
		 */
		virtual	CountedPointer<IReductionSettings> GetReductionSettings(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IReductionProcessor
	 */
	typedef CountedPointer<IReductionProcessor> spReductionProcessor;


	/**
	 * Remeshing settings for the SgRemeshingProcessing class.
	 */
	class IRemeshingSettings : public ISettingsObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRemeshingSettings is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRemeshingSettings",type)==0 )
				return true;
			return ISettingsObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRemeshingSettings pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRemeshingSettings pointer
		 * @return a pointer to the IRemeshingSettings object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRemeshingSettings *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRemeshingSettings") )
				return static_cast<IRemeshingSettings*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the level 
		 */
		virtual	real GetGroundPlaneLevel(  ) = 0;

		/**
		 * The maximum size in pixels of a generated triangle Any value from 8 pixels and above
		 * is allowed.
		 */
		virtual	unsigned int GetMaxTriangleSize(  ) = 0;

		/**
		 * The maximum size in pixels of a generated triangle Any value from 8 pixels and above
		 * is allowed.
		 */
		virtual	void SetMaxTriangleSize( unsigned int value ) = 0;

		/**
		 * Set/Get to index of axis for ground plane 0-X Axis (YZ plane), 1-Y Axis (XZ plane),
		 * 2-Z Axis (XY plane)
		 */
		virtual	void SetGroundPlaneAxisIndex( rid value ) = 0;

		/**
		 * Set/Get the level 
		 */
		virtual	void SetGroundPlaneLevel( real value ) = 0;

		/**
		 * The on-screen rendering size of the geometry. Allowed values are in the range 40
		 * to 1200 pixels.
		 */
		virtual	unsigned int GetOnScreenSize(  ) = 0;

		/**
		 * Set/Get the normal hard angle cutoff (in radians)
		 */
		virtual	void SetHardEdgeAngle( real value ) = 0;

		/**
		 * Set/Get to true to cap the mesh with a ground plane 
		 */
		virtual	void SetUseGroundPlane( bool value ) = 0;

		/**
		 * Set/Get the normal hard angle cutoff (in radians)
		 */
		virtual	real GetHardEdgeAngle(  ) = 0;

		/**
		 * Set/Get to true to cap the mesh with a ground plane 
		 */
		virtual	bool GetUseGroundPlane(  ) = 0;

		/**
		 * Set/Get to true to use negative halfspace for model, and reject the positive halfspace.
		 */
		virtual	bool GetGroundPlaneNegativeHalfspace(  ) = 0;

		/**
		 * Set/Get the TransferColors flag. If set, color channels will be sampled from the
		 * original mesh
		 */
		virtual	void SetTransferColors( bool value ) = 0;

		/**
		 * Set/Get the TransferNormals flag. If set, the normals channel will be sampled from
		 * the original mesh
		 */
		virtual	void SetTransferNormals( bool value ) = 0;

		/**
		 * Set/Get the TransferColors flag. If set, color channels will be sampled from the
		 * original mesh
		 */
		virtual	bool GetTransferColors(  ) = 0;

		/**
		 * Set/Get to index of axis for ground plane 0-X Axis (YZ plane), 1-Y Axis (XZ plane),
		 * 2-Z Axis (XY plane)
		 */
		virtual	rid GetGroundPlaneAxisIndex(  ) = 0;

		/**
		 * Set/Get to true to use negative halfspace for model, and reject the positive halfspace.
		 */
		virtual	void SetGroundPlaneNegativeHalfspace( bool value ) = 0;

		/**
		 * Set/Get the TransferNormals flag. If set, the normals channel will be sampled from
		 * the original mesh
		 */
		virtual	bool GetTransferNormals(  ) = 0;

		/**
		 * The on-screen rendering size of the geometry. Allowed values are in the range 40
		 * to 1200 pixels.
		 */
		virtual	void SetOnScreenSize( unsigned int value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRemeshingSettings
	 */
	typedef CountedPointer<IRemeshingSettings> spRemeshingSettings;

	class ISceneNode;
	class IMappingImage;
	class IRemeshingSettings;
	class IMappingImageSettings;

	/**
	 * SgRemeshingProcessor creates a new geometry that is based on the input geometry,
	 * but that is topologically based on the size of the input geometry rendered at a specific
	 * screen size, and allowed pixel error. The remesher will on demand also add texture
	 * coordinates and a mapping texture to the output remeshed geometry. The geometry 
	 * that is to be remeshed can be defined either using an in-memory  geometry data object,
	 * or using a geometry file placed on disk, the latter being the preferred method, if
	 * the source geometry is very large as it can be swapped in/out of memory as needed.
	 */
	class IRemeshingProcessor : public IProcessingObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IRemeshingProcessor is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IRemeshingProcessor",type)==0 )
				return true;
			return IProcessingObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IRemeshingProcessor pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IRemeshingProcessor pointer
		 * @return a pointer to the IRemeshingProcessor object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IRemeshingProcessor *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IRemeshingProcessor") )
				return static_cast<IRemeshingProcessor*>(ptr);
			return NULL;
			}
		/**
		 * The settings for the remeshing process.
		 */
		virtual	CountedPointer<IRemeshingSettings> GetRemeshingSettings(  ) = 0;

		/**
		 * Clears the processing, and resets all internal states. If ReduceGeometry() is called
		 * after Clear(), the Geometry object is assumed to point at a new geometry object,
		 * and is reloaded.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Get the MappingImage object that is generated if  MappingImageSettings::GenerateMappingImage
		 * is set. The object is of type SgMappingImage. A new object is generated for each
		 * call to RunProcessing().
		 */
		virtual	CountedPointer<IMappingImage> GetMappingImage(  ) = 0;

		/**
		 * Set/Get the top scene node of a scene tree used for  reduction processing. All mesh
		 * nodes in the whole scene  tree will be replaced by one proxy mesh.
		 */
		virtual	CountedPointer<ISceneNode> GetSceneRoot(  ) = 0;

		/**
		 * Set/Get the top scene node of a scene tree used for  reduction processing. All mesh
		 * nodes in the whole scene  tree will be replaced by one proxy mesh.
		 */
		virtual	void SetSceneRoot( ISceneNode *value ) = 0;

		/**
		 * The settings for the image mapping process.
		 */
		virtual	CountedPointer<IMappingImageSettings> GetMappingImageSettings(  ) = 0;

		/**
		 * Runs the remeshing of the geometry. Note that GeometryInputPath OR Geometry values
		 * must be set. If Geometry is set, it will contain the remeshed geometry on return.
		 * If GeometryOutputPath is set, the geometry  will be written to the file, regardless
		 * if GeometryInputPath or Geometry is used to input the geometry data.
		 */
		virtual	void RunProcessing(  ) = 0;

		/**
		 * Runs the remeshing of the geometry. Note that GeometryInputPath OR Geometry values
		 * must be set. If Geometry is set, it will contain the remeshed geometry on return.
		 * If GeometryOutputPath is set, the geometry  will be written to the file, regardless
		 * if GeometryInputPath or Geometry is used to input the geometry data.
		 */
		virtual	void RemeshGeometry(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IRemeshingProcessor
	 */
	typedef CountedPointer<IRemeshingProcessor> spRemeshingProcessor;

	class ISceneNode;
	class IMaterialTable;
	class ISceneBoneTable;
	class IMatrix4x4;
	class IValueArray;

	/**
	 * SgScene is the main keeper of a scene and scene graph in simplygon. The scene is
	 * self-contained, with materials, textures, etc. to describe a full scene. 
	 */
	class IScene : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IScene is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IScene",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IScene pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IScene pointer
		 * @return a pointer to the IScene object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IScene *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IScene") )
				return static_cast<IScene*>(ptr);
			return NULL;
			}
		/**
		 * Get the root node of the scene. ReGetObjectMacro( SgSceneNode , RootNode );
		 */
		virtual	CountedPointer<ISceneNode> GetRootNode(  ) = 0;

		/**
		 * Calculates the axis-aligned bounding box of the whole  scene. If the node tree contains
		 * no geometric data, or all data is hidden, the bounding box is not calcluated, and
		 * the method returns false. @returns true if the calculation succeeded
		 */
		virtual	bool CalculateExtents(  ) = 0;

		/**
		 * Revmoves all bones in the scene that are not  linked by any SgSceneMesh' geometry
		 * data. Note that the removal only removes leaf nodes in the scene.
		 */
		virtual	void RemoveUnlinkedBones(  ) = 0;

		/**
		 * Get the material table of the scene ReGetObjectMacro( ReMaterialTable , MaterialTable
		 * );
		 */
		virtual	CountedPointer<IMaterialTable> GetMaterialTable(  ) = 0;

		/**
		 * Get the inferior (minimum) extent of the scene
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

		/**
		 * Get the radius.
		 */
		virtual	real GetRadius(  ) = 0;

		/**
		 * Show a debug window with the scene graph tree. This debug method returns when the
		 * window is closed.
		 */
		virtual	void ShowSceneDebugWindow(  ) = 0;

		/**
		 * Get the superior (maximum) extent of the scene
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Adds/Removes/Gets a custom data field. To be able to add a field, it must have a
		 * name that does not  conflict with the existing custom fields.
		 */
		virtual	bool HasCustomField( const char * name ) = 0;

		/**
		 * Get the bone table of the scene ReGetObjectMacro( SgSceneBoneTable , BoneTable );
		 */
		virtual	CountedPointer<ISceneBoneTable> GetBoneTable(  ) = 0;

		/**
		 * Adds/Removes/Gets a custom data field. To be able to add a field, it must have a
		 * name that does not  conflict with the existing custom fields.
		 */
		virtual	void RemoveCustomField( const char * name ) = 0;

		/**
		 * Evaluates the global transformation of the specified node at a specific time. EvaluateDefaultGlobalTransformation
		 * is used to evaluate the default transformation, regardless of node animation. @param
		 * node the node to evaluate @param global_transform the transformation matrix that
		 * receives the global transformation 
		 */
		virtual	void EvaluateDefaultGlobalTransformation( ISceneNode *node , IMatrix4x4 *global_transform ) = 0;

		/**
		 * Clears the whole scene and releases all items in the graph.
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Gets a node defined by a path. Please note that the  path must contain names for
		 * each level to be able to find the specified node.
		 */
		virtual	CountedPointer<ISceneNode> GetNodeFromPath( const char * path ) = 0;

		/**
		 * Adds/Removes/Gets a custom data field. To be able to add a field, it must have a
		 * name that does not  conflict with the existing custom fields.
		 */
		virtual	CountedPointer<IValueArray> AddCustomField( rid base_type , const char * name , unsigned int tuple_size ) = 0;

		/**
		 * Loads a scene from file @return true on success, false if file was not found
		 */
		virtual	bool LoadFromFile( const char * path ) = 0;

		/**
		 * Adds/Removes/Gets a custom data field. To be able to add a field, it must have a
		 * name that does not  conflict with the existing custom fields.
		 */
		virtual	CountedPointer<IValueArray> GetCustomField( const char * name ) = 0;

		/**
		 * Saves a scene to file @return true on success, false if file could not be written
		 * to
		 */
		virtual	bool SaveToFile( const char * path ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IScene
	 */
	typedef CountedPointer<IScene> spScene;

	class IScene;
	class ISceneNode;
	class IMatrix4x4;

	/**
	 * SgSceneNode is the base class for all scene graph nodes in simplygon. It has relative
	 * transformation to its parent, and functions to add children to the node.
	 */
	class ISceneNode : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISceneNode is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISceneNode",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISceneNode pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISceneNode pointer
		 * @return a pointer to the ISceneNode object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISceneNode *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISceneNode") )
				return static_cast<ISceneNode*>(ptr);
			return NULL;
			}
		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	bool GetIsFrozen(  ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	bool GetIsModified(  ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	void SetOriginalName( const char * value ) = 0;

		/**
		 * Get the scene object of the node
		 */
		virtual	CountedPointer<IScene> GetScene(  ) = 0;

		/**
		 * Get the superior (maximum) extent of the node
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	void SetIsModified( bool value ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	bool GetIsVisible(  ) = 0;

		/**
		 * Get the path to this node in the scene. Please note that only paths where each node
		 * has a name can be used to get to the node using the path.
		 */
		virtual	rstring GetPath(  ) = 0;

		/**
		 * Returns true if the node has the specified child
		 */
		virtual	bool HasChild( ISceneNode *child ) = 0;

		/**
		 * Get the inferior (minimum) extent of the node
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

		/**
		 * Get the number of direct children of this node
		 */
		virtual	unsigned int GetChildCount(  ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	void SetIsVisible( bool value ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	rstring GetOriginalName(  ) = 0;

		/**
		 * Get a pointer to the i:th child of this node @param index the local index of the
		 * child (0->GetChildCount()-1)
		 */
		virtual	CountedPointer<ISceneNode> GetChild( rid index ) = 0;

		/**
		 * Finds the first child that has the name @param name the name of the child to look
		 * for
		 */
		virtual	CountedPointer<ISceneNode> FindNamedChild( const char * name ) = 0;

		/**
		 * Adds a child node to this node. If the child is already a child of another node,
		 * it is removed from this node @param child the child object.
		 */
		virtual	void AddChild( ISceneNode *child ) = 0;

		/**
		 * Removes a child from the node. @param child the child to be removed
		 */
		virtual	void RemoveChild( ISceneNode *child ) = 0;

		/**
		 * Removes all children of this node
		 */
		virtual	void RemoveChildren(  ) = 0;

		/**
		 * Removes this node from its parent.
		 */
		virtual	void RemoveFromParent(  ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	void SetIsFrozen( bool value ) = 0;

		/**
		 * Deep copies this node tree (this node and all the descendants of this node) and returns
		 * the copy top node. Please note that the copy is not added to any scene, and can be
		 * added anywhere. 
		 */
		virtual	CountedPointer<ISceneNode> NewCopy(  ) = 0;

		/**
		 * Calculates the axis-aligned bounding box of the  node and all its children. If the
		 * node tree contains no geometric data, or all data is hidden, the bounding box is
		 * not calcluated, and the method returns false. @returns true if the calculation succeeded
		 */
		virtual	bool CalculateExtents(  ) = 0;

		/**
		 * Evaluates the default global transformation of the node, regardless of node animation.
		 * @param global_transform the transformation matrix that receives the global transformation
		 * 		 */
		virtual	void EvaluateDefaultGlobalTransformation( IMatrix4x4 *global_transform ) = 0;

		/**
		 * Clones this node tree (this node and all the descendants of this node) and returns
		 * the cloned top node. Please note that the copy is not added to any scene, and can
		 * be added anywhere. Also note that the clone shares all data pointers with the original
		 * nodes, such as geometry data objects. 
		 */
		virtual	CountedPointer<ISceneNode> NewClone(  ) = 0;

		/**
		 * Get the parent node of this node
		 */
		virtual	CountedPointer<ISceneNode> GetParent(  ) = 0;

		/**
		 * Get the relative transformation of this node
		 */
		virtual	CountedPointer<IMatrix4x4> GetRelativeTransform(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ISceneNode
	 */
	typedef CountedPointer<ISceneNode> spSceneNode;

	class ISceneNode;

	/**
	 * SgSceneNodeCollection holds a number of SgSceneNode  objects. The objects can be
	 * looked up by their names (if they have  been named through ReObject::SetName() ).
	 */
	class ISceneNodeCollection : public IObjectCollection
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISceneNodeCollection is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISceneNodeCollection",type)==0 )
				return true;
			return IObjectCollection::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISceneNodeCollection pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISceneNodeCollection pointer
		 * @return a pointer to the ISceneNodeCollection object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISceneNodeCollection *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISceneNodeCollection") )
				return static_cast<ISceneNodeCollection*>(ptr);
			return NULL;
			}
		/**
		 * Adds an object object to the collection. The handle refers  to the object until it
		 * is removed from the collection again.
		 */
		virtual	rhandle AddSceneNode( ISceneNode *obj ) = 0;

		/**
		 * Returns the handle to the next item in the collection. Use  GetFirstItem and this
		 * method to step through the collection. If the return is NULL, the end of the collection
		 * has been reached.
		 */
		virtual	rhandle GetNextItem( rhandle objhandle ) = 0;

		/**
		 * Gets the object the handle is refering to.
		 */
		virtual	CountedPointer<ISceneNode> GetSceneNode( rhandle objhandle ) = 0;

		/**
		 * Removes all items from the collection.
		 */
		virtual	void RemoveAllItems(  ) = 0;

		/**
		 * Gets the object the handle is refering to.
		 */
		virtual	CountedPointer<IObject> GetItemsObject( rhandle objhandle ) = 0;

		/**
		 * Get the number of items in the collection.
		 */
		virtual	unsigned int GetItemCount(  ) = 0;

		/**
		 * Removes an item from the collection. The handle returned is the first item after
		 * this item that is in the collection. If the return is NULL, the item was the last
		 * item in the collection. Warning! The handle is invalid after the removal of the object.
		 */
		virtual	rhandle RemoveItem( rhandle objhandle ) = 0;

		/**
		 * Removes an object from the collection. Only the first occurance of the object is
		 * removed from the collection. 
		 */
		virtual	void RemoveObject( IObject *pobj ) = 0;

		/**
		 * Returns the start of the collection. The handle returned referes to the first object
		 * in the collection. If the handle is  NULL, then the collection is empty.
		 */
		virtual	rhandle GetFirstItem(  ) = 0;

		/**
		 * Returns the handle of the i:th item
		 */
		virtual	rhandle GetItem( unsigned int index ) = 0;

		/**
		 * Removes an object from the collection. Only the first occurance of the object is
		 * removed. 
		 */
		virtual	void RemoveSceneNode( ISceneNode *pobj ) = 0;

		/**
		 * Returns the object of the next item.
		 */
		virtual	CountedPointer<ISceneNode> GetNextItemAsSceneNode( rhandle *phandle ) = 0;

		/**
		 * Returns true if the object is in the collection
		 */
		virtual	bool IsSceneNodeInCollection( ISceneNode *pobj ) = 0;

		/**
		 * Finds a named object. 
		 */
		virtual	CountedPointer<ISceneNode> FindSceneNode( const char * name ) = 0;

		/**
		 * Adds an object to the collection. The handle refers to the object until the object
		 * is removed from the collection again.
		 */
		virtual	rhandle AddObject( IObject *obj ) = 0;

		/**
		 * Returns the object of the i:th item
		 */
		virtual	CountedPointer<IObject> GetItemAsObject( unsigned int index ) = 0;

		/**
		 * Returns true if the object is in the collection
		 */
		virtual	bool IsObjectInCollection( IObject *pobj ) = 0;

		/**
		 * Adds an object to the collection, as AddObject, but sorts the object into the collection
		 * based on its name.
		 */
		virtual	rhandle AddObjectSorted( IObject *obj , bool ascending ) = 0;

		/**
		 * Returns true if the item is in the collection
		 */
		virtual	bool IsItemInCollection( rhandle objhandle ) = 0;

		/**
		 * Finds a named object in the collection. The first object  with this name is returned.
		 * The search is case-sensitive.
		 */
		virtual	CountedPointer<IObject> FindObject( const char * name ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ISceneNodeCollection
	 */
	typedef CountedPointer<ISceneNodeCollection> spSceneNodeCollection;

	class IScene;
	class ISceneNode;
	class IMatrix4x4;
	class IGeometryData;

	/**
	 * SgSceneMesh is the mesh node in the simplygon scene graph
	 */
	class ISceneMesh : public ISceneNode
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISceneMesh is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISceneMesh",type)==0 )
				return true;
			return ISceneNode::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISceneMesh pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISceneMesh pointer
		 * @return a pointer to the ISceneMesh object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISceneMesh *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISceneMesh") )
				return static_cast<ISceneMesh*>(ptr);
			return NULL;
			}
		/**
		 * Deep copies this node tree (this node and all the descendants of this node) and returns
		 * the copy top node. Please note that the copy is not added to any scene, and can be
		 * added anywhere. 
		 */
		virtual	CountedPointer<ISceneNode> NewCopy(  ) = 0;

		/**
		 * Clones this node tree (this node and all the descendants of this node) and returns
		 * the cloned top node. Please note that the copy is not added to any scene, and can
		 * be added anywhere. Also note that the clone shares all data pointers with the original
		 * nodes, such as geometry data objects. 
		 */
		virtual	CountedPointer<ISceneNode> NewClone(  ) = 0;

		/**
		 * Adds a child node to this node. If the child is already a child of another node,
		 * it is removed from this node @param child the child object.
		 */
		virtual	void AddChild( ISceneNode *child ) = 0;

		/**
		 * Get a pointer to the i:th child of this node @param index the local index of the
		 * child (0->GetChildCount()-1)
		 */
		virtual	CountedPointer<ISceneNode> GetChild( rid index ) = 0;

		/**
		 * Finds the first child that has the name @param name the name of the child to look
		 * for
		 */
		virtual	CountedPointer<ISceneNode> FindNamedChild( const char * name ) = 0;

		/**
		 * Removes all children of this node
		 */
		virtual	void RemoveChildren(  ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	rstring GetOriginalName(  ) = 0;

		/**
		 * Evaluates the default global transformation of the node, regardless of node animation.
		 * @param global_transform the transformation matrix that receives the global transformation
		 * 		 */
		virtual	void EvaluateDefaultGlobalTransformation( IMatrix4x4 *global_transform ) = 0;

		/**
		 * Removes a child from the node. @param child the child to be removed
		 */
		virtual	void RemoveChild( ISceneNode *child ) = 0;

		/**
		 * Get the scene object of the node
		 */
		virtual	CountedPointer<IScene> GetScene(  ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	void SetIsVisible( bool value ) = 0;

		/**
		 * Calculates the axis-aligned bounding box of the  node and all its children. If the
		 * node tree contains no geometric data, or all data is hidden, the bounding box is
		 * not calcluated, and the method returns false. @returns true if the calculation succeeded
		 */
		virtual	bool CalculateExtents(  ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	bool GetIsVisible(  ) = 0;

		/**
		 * Removes this node from its parent.
		 */
		virtual	void RemoveFromParent(  ) = 0;

		/**
		 * Returns true if the node has the specified child
		 */
		virtual	bool HasChild( ISceneNode *child ) = 0;

		/**
		 * Get the number of direct children of this node
		 */
		virtual	unsigned int GetChildCount(  ) = 0;

		/**
		 * Get the path to this node in the scene. Please note that only paths where each node
		 * has a name can be used to get to the node using the path.
		 */
		virtual	rstring GetPath(  ) = 0;

		/**
		 * Set/Get the geometry data of the mesh node
		 */
		virtual	void SetGeometry( IGeometryData *value ) = 0;

		/**
		 * Get the relative transformation of this node
		 */
		virtual	CountedPointer<IMatrix4x4> GetRelativeTransform(  ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	void SetIsFrozen( bool value ) = 0;

		/**
		 * Get the parent node of this node
		 */
		virtual	CountedPointer<ISceneNode> GetParent(  ) = 0;

		/**
		 * Set/Get the geometry data of the mesh node
		 */
		virtual	CountedPointer<IGeometryData> GetGeometry(  ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	void SetOriginalName( const char * value ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	bool GetIsFrozen(  ) = 0;

		/**
		 * Get the superior (maximum) extent of the node
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	void SetIsModified( bool value ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	bool GetIsModified(  ) = 0;

		/**
		 * Get the inferior (minimum) extent of the node
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ISceneMesh
	 */
	typedef CountedPointer<ISceneMesh> spSceneMesh;

	class IScene;
	class ISceneNode;
	class IMatrix4x4;
	class IRealArray;

	/**
	 * SgSceneLodGroup is a LOD grouping for the scene graph
	 */
	class ISceneLodGroup : public ISceneNode
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISceneLodGroup is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISceneLodGroup",type)==0 )
				return true;
			return ISceneNode::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISceneLodGroup pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISceneLodGroup pointer
		 * @return a pointer to the ISceneLodGroup object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISceneLodGroup *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISceneLodGroup") )
				return static_cast<ISceneLodGroup*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	void SetOriginalName( const char * value ) = 0;

		/**
		 * Clones this node tree (this node and all the descendants of this node) and returns
		 * the cloned top node. Please note that the copy is not added to any scene, and can
		 * be added anywhere. Also note that the clone shares all data pointers with the original
		 * nodes, such as geometry data objects. 
		 */
		virtual	CountedPointer<ISceneNode> NewClone(  ) = 0;

		/**
		 * Get the parent node of this node
		 */
		virtual	CountedPointer<ISceneNode> GetParent(  ) = 0;

		/**
		 * Evaluates the default global transformation of the node, regardless of node animation.
		 * @param global_transform the transformation matrix that receives the global transformation
		 * 		 */
		virtual	void EvaluateDefaultGlobalTransformation( IMatrix4x4 *global_transform ) = 0;

		/**
		 * Get the relative transformation of this node
		 */
		virtual	CountedPointer<IMatrix4x4> GetRelativeTransform(  ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	rstring GetOriginalName(  ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	void SetIsVisible( bool value ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	bool GetIsVisible(  ) = 0;

		/**
		 * Get the number of direct children of this node
		 */
		virtual	unsigned int GetChildCount(  ) = 0;

		/**
		 * Calculates the axis-aligned bounding box of the  node and all its children. If the
		 * node tree contains no geometric data, or all data is hidden, the bounding box is
		 * not calcluated, and the method returns false. @returns true if the calculation succeeded
		 */
		virtual	bool CalculateExtents(  ) = 0;

		/**
		 * Returns the child node to use for rendering based on the distance parameter and the
		 * current threshold values.
		 */
		virtual	CountedPointer<ISceneNode> GetLodForDistance( real distance ) = 0;

		/**
		 * Removes all children of this node
		 */
		virtual	void RemoveChildren(  ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	bool GetIsFrozen(  ) = 0;

		/**
		 * Get the path to this node in the scene. Please note that only paths where each node
		 * has a name can be used to get to the node using the path.
		 */
		virtual	rstring GetPath(  ) = 0;

		/**
		 * Get a pointer to the i:th child of this node @param index the local index of the
		 * child (0->GetChildCount()-1)
		 */
		virtual	CountedPointer<ISceneNode> GetChild( rid index ) = 0;

		/**
		 * Removes a child from the node. @param child the child to be removed
		 */
		virtual	void RemoveChild( ISceneNode *child ) = 0;

		/**
		 * Finds the first child that has the name @param name the name of the child to look
		 * for
		 */
		virtual	CountedPointer<ISceneNode> FindNamedChild( const char * name ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	void SetIsFrozen( bool value ) = 0;

		/**
		 * Get the scene object of the node
		 */
		virtual	CountedPointer<IScene> GetScene(  ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	void SetIsModified( bool value ) = 0;

		virtual	CountedPointer<IRealArray> GetThresholds(  ) = 0;

		/**
		 * Adds a child node to this node. If the child is already a child of another node,
		 * it is removed from this node @param child the child object.
		 */
		virtual	void AddChild( ISceneNode *child ) = 0;

		/**
		 * Removes this node from its parent.
		 */
		virtual	void RemoveFromParent(  ) = 0;

		/**
		 * Get the inferior (minimum) extent of the node
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

		/**
		 * Get the superior (maximum) extent of the node
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Returns true if the node has the specified child
		 */
		virtual	bool HasChild( ISceneNode *child ) = 0;

		/**
		 * Deep copies this node tree (this node and all the descendants of this node) and returns
		 * the copy top node. Please note that the copy is not added to any scene, and can be
		 * added anywhere. 
		 */
		virtual	CountedPointer<ISceneNode> NewCopy(  ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	bool GetIsModified(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ISceneLodGroup
	 */
	typedef CountedPointer<ISceneLodGroup> spSceneLodGroup;

	class IScene;
	class ISceneNode;
	class IMatrix4x4;
	class IRidArray;

	/**
	 * SgSceneBone is the bone node in the simplygon scene graph
	 */
	class ISceneBone : public ISceneNode
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISceneBone is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISceneBone",type)==0 )
				return true;
			return ISceneNode::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISceneBone pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISceneBone pointer
		 * @return a pointer to the ISceneBone object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISceneBone *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISceneBone") )
				return static_cast<ISceneBone*>(ptr);
			return NULL;
			}
		/**
		 * Get the path to this node in the scene. Please note that only paths where each node
		 * has a name can be used to get to the node using the path.
		 */
		virtual	rstring GetPath(  ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	void SetIsFrozen( bool value ) = 0;

		/**
		 * Get the parent node of this node
		 */
		virtual	CountedPointer<ISceneNode> GetParent(  ) = 0;

		/**
		 * Get the inferior (minimum) extent of the node
		 */
		virtual	void GetInf( real dest_param[3] ) = 0;

		/**
		 * Removes this node from its parent.
		 */
		virtual	void RemoveFromParent(  ) = 0;

		virtual	bool GetLockFromBoneLOD(  ) = 0;

		/**
		 * Removes a child from the node. @param child the child to be removed
		 */
		virtual	void RemoveChild( ISceneNode *child ) = 0;

		/**
		 * Adds a child node to this node. If the child is already a child of another node,
		 * it is removed from this node @param child the child object.
		 */
		virtual	void AddChild( ISceneNode *child ) = 0;

		/**
		 * Clones this node tree (this node and all the descendants of this node) and returns
		 * the cloned top node. Please note that the copy is not added to any scene, and can
		 * be added anywhere. Also note that the clone shares all data pointers with the original
		 * nodes, such as geometry data objects. 
		 */
		virtual	CountedPointer<ISceneNode> NewClone(  ) = 0;

		/**
		 * Get the superior (maximum) extent of the node
		 */
		virtual	void GetSup( real dest_param[3] ) = 0;

		/**
		 * Returns true if the node has the specified child
		 */
		virtual	bool HasChild( ISceneNode *child ) = 0;

		/**
		 * Get/Set the IsFrozen flag. If set, the node will not be modified by processing modules.
		 */
		virtual	bool GetIsFrozen(  ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	void SetIsModified( bool value ) = 0;

		/**
		 * Looks through the hierarchy and fetches the previous BoneNode. If none is found,
		 * this is a Root-Bone-Node. -1 means it has no parent, otherwise, the ID is the parent
		 * ID.
		 */
		virtual	rid GetParentBone(  ) = 0;

		/**
		 * Get a pointer to the i:th child of this node @param index the local index of the
		 * child (0->GetChildCount()-1)
		 */
		virtual	CountedPointer<ISceneNode> GetChild( rid index ) = 0;

		/**
		 * Get/Set the IsModified flag. If set, the node is new, copied, or modified by processing
		 * modules. This can be useful to track modifications in the scene graph.
		 */
		virtual	bool GetIsModified(  ) = 0;

		/**
		 * Finds the first child that has the name @param name the name of the child to look
		 * for
		 */
		virtual	CountedPointer<ISceneNode> FindNamedChild( const char * name ) = 0;

		/**
		 * Removes all children of this node
		 */
		virtual	void RemoveChildren(  ) = 0;

		/**
		 * Deep copies this node tree (this node and all the descendants of this node) and returns
		 * the copy top node. Please note that the copy is not added to any scene, and can be
		 * added anywhere. 
		 */
		virtual	CountedPointer<ISceneNode> NewCopy(  ) = 0;

		/**
		 * Evaluates the default global transformation of the node, regardless of node animation.
		 * @param global_transform the transformation matrix that receives the global transformation
		 * 		 */
		virtual	void EvaluateDefaultGlobalTransformation( IMatrix4x4 *global_transform ) = 0;

		/**
		 * Calculates the axis-aligned bounding box of the  node and all its children. If the
		 * node tree contains no geometric data, or all data is hidden, the bounding box is
		 * not calcluated, and the method returns false. @returns true if the calculation succeeded
		 */
		virtual	bool CalculateExtents(  ) = 0;

		/**
		 * Get the number of direct children of this node
		 */
		virtual	unsigned int GetChildCount(  ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	bool GetIsVisible(  ) = 0;

		/**
		 * Looks through the sub-tree and collects the IDs of the bones found.
		 */
		virtual	void CollectAllChildBones( IRidArray *child_bones ) = 0;

		/**
		 * Get/Set the BoneId
		 */
		virtual	rid GetBoneId(  ) = 0;

		/**
		 * Get the scene object of the node
		 */
		virtual	CountedPointer<IScene> GetScene(  ) = 0;

		virtual	void SetLockFromBoneLOD( bool value ) = 0;

		/**
		 * Get the relative transformation of this node
		 */
		virtual	CountedPointer<IMatrix4x4> GetRelativeTransform(  ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	void SetOriginalName( const char * value ) = 0;

		/**
		 * Set/Get the OriginalName parameter which can be used to specify  the original name
		 * of a node, for debugging of merging purposes.  This parameter is not copied or cloned,
		 * and is not used internally by Simplygon.
		 */
		virtual	rstring GetOriginalName(  ) = 0;

		/**
		 * Get/Set the IsVisible flag. If set, the node is visible.
		 */
		virtual	void SetIsVisible( bool value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ISceneBone
	 */
	typedef CountedPointer<ISceneBone> spSceneBone;

	class ISceneBone;

	/**
	 * SgSceneBoneTable keeps information on bones used while rendering.
	 */
	class ISceneBoneTable : public ITable
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if ISceneBoneTable is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("ISceneBoneTable",type)==0 )
				return true;
			return ITable::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a ISceneBoneTable pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a ISceneBoneTable pointer
		 * @return a pointer to the ISceneBoneTable object, if the cast can be made, and a NULL pointer otherwise
		 */
		static ISceneBoneTable *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("ISceneBoneTable") )
				return static_cast<ISceneBoneTable*>(ptr);
			return NULL;
			}
		/**
		 * Returns the i:th bone, using the id of the bone
		 */
		virtual	CountedPointer<ISceneBone> GetBone( rid id ) = 0;

		/**
		 * Clears the table, and removes all items. If AddItem is called after Clear, the item
		 * ids will start over from 0
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Finds the id of a bone in the table, using the bone name. If multiple bones have
		 * the same name, the first will be returned. If the bone was not found, the return
		 * will be -1.
		 */
		virtual	rid FindBoneId( const char * name ) = 0;

		/**
		 * Adds a bone to the table. The returned value is  the id of the bone within the table.
		 */
		virtual	rid AddBone( ISceneBone *bone ) = 0;

		/**
		 * Finds the id of an item in the table, using the name. If multiple items have the
		 * same name, the first will be returned. If no item was found, the return will be -1.
		 */
		virtual	rid FindItemId( const char * name ) = 0;

		/**
		 * Finds a named item in the table. If multiple items  have the same name, the first
		 * will be returned. If no item  was found, the return will be NULL.
		 */
		virtual	CountedPointer<IObject> FindItem( const char * name ) = 0;

		/**
		 * Adds an item to the table. The returned value is  the id of the item within the table,
		 * which can be used to retrieve the item using GetItem(). 
		 */
		virtual	rid AddItem( IObject *item ) = 0;

		/**
		 * Removes a bone. If GetBone is called with the bone id, the value returned will be
		 * NULL.
		 */
		virtual	void RemoveBone( rid id ) = 0;

		/**
		 * Returns the number of item id:s in the table. If an item has been removed, using
		 * RemoveItem, the id is still  counted in this method. GetIdsCount also equals the
		 * id that will be applied to the next item added to the table.
		 */
		virtual	unsigned int GetItemsCount(  ) = 0;

		/**
		 * Returns the number of bones in the table
		 */
		virtual	unsigned int GetBonesCount(  ) = 0;

		/**
		 * Sets an item in the table. The id must exist in the table.
		 */
		virtual	void SetItem( rid id , IObject *item ) = 0;

		/**
		 * Finds a bone in the table, using the bone name. If multiple bones have the same name,
		 * the first will be returned. If the bone was not found, the return will be NULL.
		 */
		virtual	CountedPointer<ISceneBone> FindBone( const char * name ) = 0;

		/**
		 * Returns the i:th item, using the id of the item
		 */
		virtual	CountedPointer<IObject> GetItem( rid id ) = 0;

		/**
		 * Removes an item, and invalidates the id. The id will not point at a valid item, until
		 * Clear is called and new items are added up until the id.
		 */
		virtual	void RemoveItem( rid id ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an ISceneBoneTable
	 */
	typedef CountedPointer<ISceneBoneTable> spSceneBoneTable;

	class IImageData;

	/**
	 * class for loading image data from different file formats
	 */
	class IImageDataImporter : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IImageDataImporter is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IImageDataImporter",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IImageDataImporter pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IImageDataImporter pointer
		 * @return a pointer to the IImageDataImporter object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IImageDataImporter *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IImageDataImporter") )
				return static_cast<IImageDataImporter*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get MaxHeight. If set to a value above 0, the image that is higher will be rescaled
		 * to MaxHeight
		 */
		virtual	void SetMaxHeight( unsigned int value ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. 
		 */
		virtual	void SetImportFilePath( const char * value ) = 0;

		/**
		 * Runs the import. Note that all parameters must be setup before importing.
		 */
		virtual	bool RunImport(  ) = 0;

		/**
		 * Get the imported image data
		 */
		virtual	CountedPointer<IImageData> GetImage(  ) = 0;

		/**
		 * Set/Get the main import file path. This must always be set. 
		 */
		virtual	rstring GetImportFilePath(  ) = 0;

		/**
		 * Set/Get the override file extension. This is specified including the dot, eg ".tga"
		 */
		virtual	rstring GetExtensionOverride(  ) = 0;

		/**
		 * Close any open file or stream, release any allocated data 
		 */
		virtual	void Clear(  ) = 0;

		/**
		 * Set/Get the override file extension. This is specified including the dot, eg ".tga"
		 */
		virtual	void SetExtensionOverride( const char * value ) = 0;

		/**
		 * Set/Get MaxHeight. If set to a value above 0, the image that is higher will be rescaled
		 * to MaxHeight
		 */
		virtual	unsigned int GetMaxHeight(  ) = 0;

		/**
		 * Set/Get CapToPowerOfTwo. If set, the width and height of the loaded image will be
		 * capped to the closest power of two below the size of the image
		 */
		virtual	void SetCapToPowerOfTwo( bool value ) = 0;

		/**
		 * Set/Get CapToPowerOfTwo. If set, the width and height of the loaded image will be
		 * capped to the closest power of two below the size of the image
		 */
		virtual	bool GetCapToPowerOfTwo(  ) = 0;

		/**
		 * Set/Get MaxWidth. If set to a value above 0, the image that is wider will be rescaled
		 * to MaxWidth
		 */
		virtual	unsigned int GetMaxWidth(  ) = 0;

		/**
		 * Set/Get MaxWidth. If set to a value above 0, the image that is wider will be rescaled
		 * to MaxWidth
		 */
		virtual	void SetMaxWidth( unsigned int value ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IImageDataImporter
	 */
	typedef CountedPointer<IImageDataImporter> spImageDataImporter;

	class IGeometryData;
	class IRealArray;

	/**
	 * Computes texture coordinates for arbitrary geometry.
	 */
	class IParameterizer : public IObject
	{
	public:
		/**
		 * GetClass returns the name of the class of the object.
		 * @return the name of the actual class of the object, as a const rchar string
		 */
		virtual const rchar *GetClass();

		/** 
		 * The IsA function returns true if the object is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the object is, or is a decendant of
		 * @return true if the object is of the specified class, false if not
		 */
		virtual bool IsA( const rchar *type ) const;

		/**
		 * The IsClassA function returns true if IParameterizer is a or is a decendant of the class named as the type parameter
		 * @param type is the name of the class to check if the class is, or is a decendant of
		 * @return true if the class is of the specified class, false if not
		 */
		static bool IsClassA( const rchar *type )
			{
			if( !type )
				return false;
			if( strcmp("IParameterizer",type)==0 )
				return true;
			return IObject::IsClassA(type);
			}
		/** 
		 * SafeCast makes sure the pointer is of a class that can be cast into a IParameterizer pointer, and if this is possible, returns the cast pointer. 
		 * @param ptr is the pointer to be cast into a IParameterizer pointer
		 * @return a pointer to the IParameterizer object, if the cast can be made, and a NULL pointer otherwise
		 */
		static IParameterizer *SafeCast( IObject *ptr )
			{
			if( ptr && ptr->IsA("IParameterizer") )
				return static_cast<IParameterizer*>(ptr);
			return NULL;
			}
		/**
		 * Set/Get the packing performance setting the value is in the range 0->1, where 0 has
		 * the lowest packing efficiency, but runs the fastest, and 1 has the highest packing
		 * efficiency but runs slower.
		 */
		virtual	void SetPackingEfficency( real value ) = 0;

		/**
		 * The height of the texture to use.
		 */
		virtual	unsigned int GetTextureHeight(  ) = 0;

		/**
		 * Set/Get the FeatureFlagsMask. Any edge that has  one of the flags present as a feature
		 * edge, will have the edge forced as a parameterization feature.
		 */
		virtual	void SetFeatureFlagsMask( unsigned int value ) = 0;

		/**
		 * Set/Get the packing performance setting the value is in the range 0->1, where 0 has
		 * the lowest packing efficiency, but runs the fastest, and 1 has the highest packing
		 * efficiency but runs slower.
		 */
		virtual	real GetPackingEfficency(  ) = 0;

		/**
		 * Parameterizes the geometry.
		 */
		virtual	bool Parameterize( IGeometryData *geom , IRealArray *arr ) = 0;

		/**
		 * The width of the texture to use.
		 */
		virtual	unsigned int GetTextureWidth(  ) = 0;

		/**
		 * The height of the texture to use.
		 */
		virtual	void SetTextureHeight( unsigned int value ) = 0;

		/**
		 * The width of the texture to use.
		 */
		virtual	void SetTextureWidth( unsigned int value ) = 0;

		/**
		 * The minimum number of pixels between charts.
		 */
		virtual	void SetGutterSpace( unsigned int value ) = 0;

		/**
		 * The maximum allowed texture stretch. Range 0->1
		 */
		virtual	void SetMaxStretch( real value ) = 0;

		/**
		 * The minimum number of pixels between charts.
		 */
		virtual	unsigned int GetGutterSpace(  ) = 0;

		/**
		 * Set/Get the FeatureFlagsMask. Any edge that has  one of the flags present as a feature
		 * edge, will have the edge forced as a parameterization feature.
		 */
		virtual	unsigned int GetFeatureFlagsMask(  ) = 0;

		/**
		 * The maximum allowed texture stretch. Range 0->1
		 */
		virtual	real GetMaxStretch(  ) = 0;

	};

	/**
	 * a CounterPointer smart pointer to an IParameterizer
	 */
	typedef CountedPointer<IParameterizer> spParameterizer;

	class ISimplygonSDK
	{
	public:
		/**
		 * Creates and returns a counted pointer to an object of type IStringArray
		 * @return the created object
		 */
		virtual CountedPointer<IStringArray> CreateStringArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IBoolArray
		 * @return the created object
		 */
		virtual CountedPointer<IBoolArray> CreateBoolArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ICharArray
		 * @return the created object
		 */
		virtual CountedPointer<ICharArray> CreateCharArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IDoubleArray
		 * @return the created object
		 */
		virtual CountedPointer<IDoubleArray> CreateDoubleArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IFloatArray
		 * @return the created object
		 */
		virtual CountedPointer<IFloatArray> CreateFloatArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IIntArray
		 * @return the created object
		 */
		virtual CountedPointer<IIntArray> CreateIntArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ILongArray
		 * @return the created object
		 */
		virtual CountedPointer<ILongArray> CreateLongArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRcharArray
		 * @return the created object
		 */
		virtual CountedPointer<IRcharArray> CreateRcharArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRealArray
		 * @return the created object
		 */
		virtual CountedPointer<IRealArray> CreateRealArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRhandleArray
		 * @return the created object
		 */
		virtual CountedPointer<IRhandleArray> CreateRhandleArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRidArray
		 * @return the created object
		 */
		virtual CountedPointer<IRidArray> CreateRidArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IShortArray
		 * @return the created object
		 */
		virtual CountedPointer<IShortArray> CreateShortArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IUnsignedCharArray
		 * @return the created object
		 */
		virtual CountedPointer<IUnsignedCharArray> CreateUnsignedCharArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IUnsignedIntArray
		 * @return the created object
		 */
		virtual CountedPointer<IUnsignedIntArray> CreateUnsignedIntArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IUnsignedLongArray
		 * @return the created object
		 */
		virtual CountedPointer<IUnsignedLongArray> CreateUnsignedLongArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IUnsignedShortArray
		 * @return the created object
		 */
		virtual CountedPointer<IUnsignedShortArray> CreateUnsignedShortArray() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IFieldData
		 * @return the created object
		 */
		virtual CountedPointer<IFieldData> CreateFieldData() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IGeometryData
		 * @return the created object
		 */
		virtual CountedPointer<IGeometryData> CreateGeometryData() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IPackedGeometryData
		 * @return the created object
		 */
		virtual CountedPointer<IPackedGeometryData> CreatePackedGeometryData() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IImageData
		 * @return the created object
		 */
		virtual CountedPointer<IImageData> CreateImageData() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ITable
		 * @return the created object
		 */
		virtual CountedPointer<ITable> CreateTable() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IMaterial
		 * @return the created object
		 */
		virtual CountedPointer<IMaterial> CreateMaterial() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IMaterialTable
		 * @return the created object
		 */
		virtual CountedPointer<IMaterialTable> CreateMaterialTable() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IMatrix4x4
		 * @return the created object
		 */
		virtual CountedPointer<IMatrix4x4> CreateMatrix4x4() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ITransform3
		 * @return the created object
		 */
		virtual CountedPointer<ITransform3> CreateTransform3() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IMappingImage
		 * @return the created object
		 */
		virtual CountedPointer<IMappingImage> CreateMappingImage() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IGeometryGroup
		 * @return the created object
		 */
		virtual CountedPointer<IGeometryGroup> CreateGeometryGroup() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IObjectCollection
		 * @return the created object
		 */
		virtual CountedPointer<IObjectCollection> CreateObjectCollection() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IGeometryDataCollection
		 * @return the created object
		 */
		virtual CountedPointer<IGeometryDataCollection> CreateGeometryDataCollection() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IWavefrontImporter
		 * @return the created object
		 */
		virtual CountedPointer<IWavefrontImporter> CreateWavefrontImporter() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IWavefrontExporter
		 * @return the created object
		 */
		virtual CountedPointer<IWavefrontExporter> CreateWavefrontExporter() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IBinaryImporter
		 * @return the created object
		 */
		virtual CountedPointer<IBinaryImporter> CreateBinaryImporter() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IBinaryExporter
		 * @return the created object
		 */
		virtual CountedPointer<IBinaryExporter> CreateBinaryExporter() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IGeometryValidator
		 * @return the created object
		 */
		virtual CountedPointer<IGeometryValidator> CreateGeometryValidator() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IColorCaster
		 * @return the created object
		 */
		virtual CountedPointer<IColorCaster> CreateColorCaster() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type INormalCaster
		 * @return the created object
		 */
		virtual CountedPointer<INormalCaster> CreateNormalCaster() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type INormalRepairer
		 * @return the created object
		 */
		virtual CountedPointer<INormalRepairer> CreateNormalRepairer() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IDisplacementCaster
		 * @return the created object
		 */
		virtual CountedPointer<IDisplacementCaster> CreateDisplacementCaster() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IGeometryTangentCalculator
		 * @return the created object
		 */
		virtual CountedPointer<IGeometryTangentCalculator> CreateGeometryTangentCalculator() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRepairSettings
		 * @return the created object
		 */
		virtual CountedPointer<IRepairSettings> CreateRepairSettings() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IBoneSettings
		 * @return the created object
		 */
		virtual CountedPointer<IBoneSettings> CreateBoneSettings() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IReductionSettings
		 * @return the created object
		 */
		virtual CountedPointer<IReductionSettings> CreateReductionSettings() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type INormalCalculationSettings
		 * @return the created object
		 */
		virtual CountedPointer<INormalCalculationSettings> CreateNormalCalculationSettings() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IMappingImageSettings
		 * @return the created object
		 */
		virtual CountedPointer<IMappingImageSettings> CreateMappingImageSettings() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IReductionProcessor
		 * @return the created object
		 */
		virtual CountedPointer<IReductionProcessor> CreateReductionProcessor() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRemeshingSettings
		 * @return the created object
		 */
		virtual CountedPointer<IRemeshingSettings> CreateRemeshingSettings() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IRemeshingProcessor
		 * @return the created object
		 */
		virtual CountedPointer<IRemeshingProcessor> CreateRemeshingProcessor() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IScene
		 * @return the created object
		 */
		virtual CountedPointer<IScene> CreateScene() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ISceneNode
		 * @return the created object
		 */
		virtual CountedPointer<ISceneNode> CreateSceneNode() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ISceneNodeCollection
		 * @return the created object
		 */
		virtual CountedPointer<ISceneNodeCollection> CreateSceneNodeCollection() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ISceneMesh
		 * @return the created object
		 */
		virtual CountedPointer<ISceneMesh> CreateSceneMesh() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ISceneLodGroup
		 * @return the created object
		 */
		virtual CountedPointer<ISceneLodGroup> CreateSceneLodGroup() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ISceneBone
		 * @return the created object
		 */
		virtual CountedPointer<ISceneBone> CreateSceneBone() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type ISceneBoneTable
		 * @return the created object
		 */
		virtual CountedPointer<ISceneBoneTable> CreateSceneBoneTable() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IImageDataImporter
		 * @return the created object
		 */
		virtual CountedPointer<IImageDataImporter> CreateImageDataImporter() = 0;

		/**
		 * Creates and returns a counted pointer to an object of type IParameterizer
		 * @return the created object
		 */
		virtual CountedPointer<IParameterizer> CreateParameterizer() = 0;

		/**
		 * Sets the callback used to report errors
		 * @param handler is the new error handler object
		 * @return the previously registered handler object
		 */
		virtual rerrorhandler *SetErrorHandler( rerrorhandler *handler ) = 0;
		/**
		 * Returns the version of the Simplygon library as a string
		 * @return the library version
		 */
		virtual const rchar *GetVersion() const = 0;
		/**
		 * Sets a named global library setting
		 */
		virtual void SetGlobalSetting( const char *setting_name , intptr_t value ) const = 0;
		/**
		 * Gets a named global library setting
		 */
		virtual intptr_t GetGlobalSetting( const char *setting_name ) const = 0;
	};

	/**
	 * SafeCast casts a pointer to an API object into a pointer to another API class type. The cast is only 
	 * made if the object is of the type it is cast into, else NULL is returned.
	 * @param ptr is the pointer that is to be cast
	 * @return a counted pointer of the template T cast type 
	 */
	template <class T> CountedPointer<T> SafeCast( IObject *ptr )
		{
		return CountedPointer<T>(T::SafeCast(ptr)); 
		}

	/**
	 * UnsafeCast casts a pointer to an API object into a pointer to another API class type. The pointer is cast
	 * using static_cast, and is done regardless of if the object is actually of the type it is cast into
	 * @param ptr is the pointer that is to be cast
	 * @return a counted pointer of the template T cast type 
	 */
	 template <class T> CountedPointer<T> UnsafeCast( IObject *ptr )
		{
		return CountedPointer<T>(static_cast<T*>(ptr));
		}

	/**
	 * Cast casts a pointer to an API object into a pointer to another API class type. In debug 
	 * mode (where the _DEBUG define exists), the cast is only made if the object is of the 
	 * type it is cast into, else NULL is returned.
	 * In release mode, the pointer is cast statically without checking.
	 * @param ptr is the pointer that is to be cast
	 * @return a counted pointer of the template T cast type 
	 */
	template <class T> CountedPointer<T> Cast( IObject *ptr )
		{
	#ifdef _DEBUG
		return SafeCast<T>(ptr);
	#else
		return UnsafeCast<T>(ptr);
	#endif
		}	

	/**
	 * Returns the API interface version hash as a string
	 * @return the API interface version hash
	 */
	inline const rchar *GetInterfaceVersionHash()
		{
		return "1B2BFDECB0DBE071E9DC9728F4BD6AEE6126BC3CFCB9DCDE80E2AA41228759C2";
		}

};

#endif//__SimplygonSDK_h__
