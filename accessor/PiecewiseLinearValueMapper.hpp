//
// Created by zhouj on 2022/12/10.
//

#ifndef WWMOTOR_LIBS_WWDEVICE_ACCESSOR_PIECEWISELINEARVALUEMAPPER_HPP_
#define WWMOTOR_LIBS_WWDEVICE_ACCESSOR_PIECEWISELINEARVALUEMAPPER_HPP_

#include <cmath>
#include "base.hpp"
#include "ValueMapper.hpp"
namespace wibot
{
	namespace accessor
	{

#define PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K 0

		template<size_t N>
		struct PiecewiseLinearValueMapperConfig
		{
			float value_per_unit;
			uint32_t zero_offset;
			uint32_t in_wrap;
			float out_wrap;
			uint32_t in_points[N];
			float out_points[N];
#if PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K
			// float out_k[N];
#endif
		};

		/**
		 * @brief Piecewise linear value mapper
		 * Map raw value to zero to wrap value, with zero offset.
		 * @tparam N
		 */
		template<size_t N>
		class PiecewiseLinearValueMapper : public ValueMapper, public Configurable<PiecewiseLinearValueMapperConfig<N>>
		{

		 public:

			/**
			 *	@brief	开始校准.
			 *	@note Make sure the zero offset is correct before calibration.
			 */
			void calibrate_begin();
			void calibrate(uint8_t step, uint32_t in_value, float out_value);
			void calibrate_end();
			void set_zero_offset(uint32_t zero_offset);
			float value_get(uint32_t in_value) override;

			void apply_config(PiecewiseLinearValueMapperConfig<N> config);
		 private:
			float _last_value;

			uint8_t _cal_count[N];

		 private:
			uint32_t wrap(int32_t value);

		};
		template<size_t N>
		void PiecewiseLinearValueMapper<N>::calibrate_end()
		{

			for (int i = 0; i < N - 1; ++i)
			{
				this->config.in_points[i] /= this->_cal_count[i];
				this->config.out_points[i] /= this->_cal_count[i];
#if PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K
				this->config.out_k[i] = (this->config.out_points[i + 1] - this->config.out_points[i])
					/ (this->config.in_points[i + 1] - this->config.in_points[i]);
#endif
			}
			this->config.in_points[N - 1] /= this->_cal_count[N - 1];
			this->config.out_points[N - 1] /= this->_cal_count[N - 1];
#if PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K
			this->config.out_k[N - 1] = (this->config.out_wrap - this->config.out_points[N - 1])
				/ (this->config.in_wrap - this->config.in_points[N - 1]);
#endif
		}
		template<size_t N>
		uint32_t PiecewiseLinearValueMapper<N>::wrap(int32_t value)
		{
			if (value > (int32_t)this->config.in_wrap)
			{
				value -= (int32_t)this->config.in_wrap;
			}
			else if (value < 0)
			{
				value += (int32_t)this->config.in_wrap;
			}
			return value;
		}
		template<size_t N>
		void PiecewiseLinearValueMapper<N>::set_zero_offset(uint32_t zero_offset)
		{
			this->config.zero_offset = zero_offset;
		}
		template<size_t N>
		void PiecewiseLinearValueMapper<N>::apply_config(PiecewiseLinearValueMapperConfig<N> config)
		{
			this->config = config;
		}

		template<size_t N>
		void PiecewiseLinearValueMapper<N>::calibrate(uint8_t step, uint32_t in_value, float out_value)
		{
			auto wrap_value = wrap((int32_t)in_value - (int32_t)this->config.zero_offset);
			this->config.in_points[step] += wrap_value;
			this->config.out_points[step] += out_value;
			this->_cal_count[step] += 1;
		}

		template<size_t N>
		void PiecewiseLinearValueMapper<N>::calibrate_begin()
		{
			for (int i = 0; i < N; ++i)
			{
				this->config.in_points[i] = 0;
				this->config.out_points[i] = 0;
#if PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K
				this->config.out_k[i] = 0;
#endif
				this->_cal_count[i] = 0;
			}
		}
		template<size_t N>
		float PiecewiseLinearValueMapper<N>::value_get(uint32_t raw_value)
		{
			auto wrap_value = wrap((int32_t)raw_value - (int32_t)this->config.zero_offset);
			for (size_t i = 0; i < N - 1; ++i)
			{
				if (wrap_value >= this->config.in_points[i] && wrap_value <= this->config.in_points[i + 1])
				{
#if PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K
					_last_value =
						this->config.out_points[i] + this->config.out_k[i] * (wrap_value - this->config.in_points[i]);
#else
					_last_value =
						this->config.out_points[i] + (this->config.out_points[i + 1] - this->config.out_points[i])
							/ (this->config.in_points[i + 1] - this->config.in_points[i])
							* (wrap_value - this->config.in_points[i]);
#endif
					return _last_value;
				}
			}
			if (wrap_value >= this->config.in_points[N - 1])
			{
#if PIECEWISE_LINEAR_VALUE_MAPPER_ENABLE_K
				_last_value =
					this->config.out_points[N - 1]
						+ this->config.out_k[N - 1] * (wrap_value - this->config.in_points[N - 1]);
#else
				_last_value =
					this->config.out_points[N - 1]
						+ (this->config.out_wrap - this->config.out_points[N - 1])
							/ (this->config.in_wrap - this->config.in_points[N - 1])
							* (wrap_value - this->config.in_points[N - 1]);
#endif
				return _last_value;
			}
			return NAN;
		}

	} // wibot
} // accessor

#endif //WWMOTOR_LIBS_WWDEVICE_ACCESSOR_PIECEWISELINEARVALUEMAPPER_HPP_
