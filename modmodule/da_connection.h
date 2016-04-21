/*  
 *  
 *   MIT License
 *   
 *   Copyright (c) 2016 Francesco Mannella and Daniele Caligiore
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 *  
 */

#ifndef DA_CONNECTION
#define DA_CONNECTION

#include "connection.h"
#include "modulatory_connection.h"
#include <vector>
    
/*
 *  Implements modulatory synapse to reproduce DIR and D2R dopaminergic modulation as in
 *  Humphries et al. 2006 (http://dx.doi.org/10.1523/JNEUROSCI.3486-06.2006).
 */

namespace mynest
{

    /*
    *  The class D1Connection implements a dopaminergic synapse in which   
    *  the information from the volume transmitter modulates the amplitude of the weight as
    *  in D1R synapses in the model by Humphries et al. 2006 (http://dx.doi.org/10.1523/JNEUROSCI.3486-06.2006).
    *  
    *  weight = initial_weight*(1 + alpha*modulation)
    *
    *  Parameters:
    *      initial_weight =>  the baseline value which has to be multiplied times the *modulation* 
    *      alpha => amplitude of the modulated increment
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


    /*
    *  The class D1Connection implements a dopaminergic synapse in which   
    *  the information from the volume transmitter modulates the amplitude of the weight as
    *  in D2R synapses in the model by Humphries et al. 2006 (http://dx.doi.org/10.1523/JNEUROSCI.3486-06.2006).
    *  
    *  weight = initial_weight*(1 - alpha*modulation)
    *
    *  Parameters:
    *      initial_weight =>  the baseline value which has to be multiplied times the *modulation* 
    *      alpha => amplitude of the modulated decrease
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
