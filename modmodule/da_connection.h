#ifndef DA_CONNECTION
#define DA_CONNECTION

#include "connection.h"
#include "modulatory_connection.h"
#include <vector>

namespace mynest
{

    /**
     * D1 connection
     * A third moduatory neuron can change the 
     * strength of the weights 
     */
    template < typename targetidentifierT >
        class D1Connection : public ModulatoryConnection< targetidentifierT >
    {
        private:
            
            nest::double_t alpha;

        public:
            
                //! Shortcut for base class
            typedef ModulatoryConnection< targetidentifierT > ConnectionBase;


            D1Connection() 
                : ConnectionBase(), alpha(1.0)
            {
            }

            // The following methods contain mostly fixed code to forward the corresponding
            // tasks to corresponding methods in the base class and the w_ data member holding
            // the weight.

            //! Store connection status information in dictionary
            void get_status( DictionaryDatum& d ) const;

            /**
             * Set connection status.
             *
             * @param d Dictionary with new parameter values
             * @param cm ConnectorModel is passed along to validate new delay values
             */
            void set_status( const DictionaryDatum& d, nest::ConnectorModel& cm );

            //! Allows efficient initialization on contstruction
            void  set_alpha( nest::double_t alpha_ )
            {
                alpha = alpha_;
            }

            virtual nest::double_t compute_modulation(nest::double_t modulation)
            {
                return 1.0 + alpha*modulation;
            }
    };


    template < typename targetidentifierT >
        void D1Connection< targetidentifierT >::get_status( DictionaryDatum& d ) const
        {
            ConnectionBase::get_status( d );
            def< nest::double_t >( d, "alpha", alpha );
            def< nest::long_t >( d, nest::names::size_of, sizeof( *this ) );
        }

    template < typename targetidentifierT >
        void D1Connection< targetidentifierT >::set_status( const DictionaryDatum& d,
                nest::ConnectorModel& cm )
        {
            ConnectionBase::set_status( d, cm );
            updateValue< nest::double_t >( d, "alpha", alpha );
        }


    /**
     * D2 connection
     * A third moduatory neuron can change the 
     * strength of the weights 
     */
    template < typename targetidentifierT >
        class D2Connection : public ModulatoryConnection< targetidentifierT >
    {
        private:
            
            nest::double_t alpha;

        public:
            
                //! Shortcut for base class
            typedef ModulatoryConnection< targetidentifierT > ConnectionBase;


            D2Connection() 
                : ConnectionBase(), alpha(1.0)
            {
            }

            // The following methods contain mostly fixed code to forward the corresponding
            // tasks to corresponding methods in the base class and the w_ data member holding
            // the weight.

            //! Store connection status information in dictionary
            void get_status( DictionaryDatum& d ) const;

            /**
             * Set connection status.
             *
             * @param d Dictionary with new parameter values
             * @param cm ConnectorModel is passed along to validate new delay values
             */
            void set_status( const DictionaryDatum& d, nest::ConnectorModel& cm );

            //! Allows efficient initialization on contstruction
            void  set_alpha( nest::double_t alpha_ )
            {
                alpha = alpha_;
            }

            virtual nest::double_t compute_modulation(nest::double_t modulation)
            {
                return 1.0 - alpha*modulation;
            }
    };


    template < typename targetidentifierT >
        void D2Connection< targetidentifierT >::get_status( DictionaryDatum& d ) const
        {
            ConnectionBase::get_status( d );
            def< nest::double_t >( d, "alpha", alpha );
            def< nest::long_t >( d, nest::names::size_of, sizeof( *this ) );
        }

    template < typename targetidentifierT >
        void D2Connection< targetidentifierT >::set_status( const DictionaryDatum& d,
                nest::ConnectorModel& cm )
        {
            ConnectionBase::set_status( d, cm );
            updateValue< nest::double_t >( d, "alpha", alpha );
        }




} // namespace nest

#endif // DA_CONNECTION
