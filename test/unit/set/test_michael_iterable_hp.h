/*
    This file is a part of libcds - Concurrent Data Structures library

    (C) Copyright Maxim Khizhinsky (libcds.dev@gmail.com) 2006-2016

    Source code repo: http://github.com/khizmax/libcds/
    Download: http://sourceforge.net/projects/libcds/files/

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CDSUNIT_SET_TEST_SET_HP_H
#define CDSUNIT_SET_TEST_SET_HP_H

#include "test_michael_iterable.h"

namespace cds_test {

    class michael_iterable_set_hp: public michael_iterable_set
    {
        typedef michael_iterable_set base_class;

    protected:
        template <typename Set>
        void test( Set& s )
        {
            // Precondition: set is empty
            // Postcondition: set is empty

            EXPECT_TRUE( s.empty());
            EXPECT_CONTAINER_SIZE( s, 0 );

            base_class::test( s );

            typedef typename Set::value_type value_type;

            size_t const nSetSize = kSize;
            std::vector< value_type > data;
            std::vector< size_t> indices;
            data.reserve( kSize );
            indices.reserve( kSize );
            for ( size_t key = 0; key < kSize; ++key ) {
                data.push_back( value_type( static_cast<int>(key)));
                indices.push_back( key );
            }
            shuffle( indices.begin(), indices.end());

            for ( auto& i : data ) {
                EXPECT_TRUE( s.insert( i ));
            }
            EXPECT_FALSE( s.empty());
            EXPECT_CONTAINER_SIZE( s, nSetSize );

            // iterator test
            for ( auto it = s.begin(); it != s.end(); ++it ) {
                it->nFindCount = it->key() * 3;
            }

            for ( auto it = s.cbegin(); it != s.cend(); ++it ) {
                EXPECT_EQ( it->nFindCount, static_cast<size_t>( it->key() * 3 ));
            }

            typedef typename Set::guarded_ptr guarded_ptr;
            guarded_ptr gp;

            // get()
            for ( auto idx : indices ) {
                auto& i = data[idx];

                EXPECT_TRUE( !gp );
                switch ( idx % 3 ) {
                case 0:
                    gp = s.get( i.key());
                    ASSERT_FALSE( !gp );
                    break;
                case 1:
                    gp = s.get( i );
                    ASSERT_FALSE( !gp );
                    break;
                case 2:
                    gp = s.get_with( other_item( i.key()), other_less());
                    ASSERT_FALSE( !gp );
                }
                EXPECT_EQ( gp->key(), i.key());
                EXPECT_EQ( gp->nFindCount, static_cast<size_t>( i.key() * 3 ));
                gp->nFindCount *= 2;

                gp.release();
            }

            // extract()
            for ( auto idx : indices ) {
                auto& i = data[idx];

                EXPECT_TRUE( !gp );
                switch ( idx % 3 ) {
                case 0:
                    gp = s.extract( i.key());
                    ASSERT_FALSE( !gp );
                    break;
                case 1:
                    gp = s.extract( i );
                    ASSERT_FALSE( !gp );
                    break;
                case 2:
                    gp = s.extract_with( other_item( i.key()), other_less());
                    ASSERT_FALSE( !gp );
                    break;
                }
                EXPECT_EQ( gp->key(), i.key());
                EXPECT_EQ( gp->nFindCount, static_cast<size_t>( i.key() * 6 ));

                switch ( idx % 3 ) {
                case 0:
                    gp = s.extract( i.key());
                    break;
                case 1:
                    gp = s.extract( i );
                    break;
                case 2:
                    gp = s.extract_with( other_item( i.key()), other_less());
                    break;
                }
                EXPECT_TRUE( !gp );
            }

            EXPECT_TRUE( s.empty());
            EXPECT_CONTAINER_SIZE( s, 0 );
        }

    };
} // namespace cds_test

#endif // CDSUNIT_SET_TEST_SET_HP_H
