/*****************************************************************************
 * Copyright (c) 2013-2016 Intel Corporation
 * All rights reserved.
 *
 * WARRANTY DISCLAIMER
 *
 * THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
 * MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Intel Corporation is the author of the Materials, and requests that all
 * problem reports or change requests be submitted to it directly
 *****************************************************************************/

__kernel void updateSphere(
			__global float* x, __global float* y, __global float* z, 
            __global float* vx, __global float* vy, __global float* vz, 
			__global float* r, __global float* m,
			__global float* nx, __global float* ny, __global float* nz, 
            __global float* nvx, __global float* nvy, __global float* nvz,
			uint size) {
	const int index = get_global_id(0);
	const float g = -0.0009;								// �������ٶ�
	const float a = 0.005, b = 0.002, c = 0.003;			// ����ϵ��
	const float interval = 0.5;								// ʱ����
	const float mass = 2;									// С������
	const float x_maxm = 1.0, y_maxm = 1.0, z_maxm = 1.0;	// �߽�ֵ
	const float decay = -0.9;								// ˥��ϵ��
	float force_x = 0.0, force_y = 0.0, force_z = 0.0;		// ����

	nx[index] = x[index];
	ny[index] = y[index];
	nz[index] = z[index];

	// spheres[index]��spheres[i]����С�����ײ
	for (int i = 0; i < size; ++i) {
		if (i == index) {
			continue;
		}

		float dx = x[index] - x[i];
		float dy = y[index] - y[i];
		float dz = z[index] - z[i];
		float dist = sqrt(dx * dx + dy * dy + dz * dz + 0.00000001);
		float delta = dist - (r[index] + r[i]);

		// С��֮�䷢����ײ
		if (delta < 0) {
			// ����С����ײ���λ��
			float norm_x = dx / dist;
			float norm_y = dy / dist;
			float norm_z = dz / dist;
			nx[index] += -norm_x * delta / 4;
			ny[index] += -norm_y * delta / 4;
			nz[index] += -norm_z * delta / 4;

			// ����С������
			float dvx = vx[index] - vx[i];
			float dvy = vy[index] - vy[i];
			float dvz = vz[index] - vz[i];
			// �����ٶ�
			float norm_v = dvx * norm_x + dvy * norm_y + dvz * norm_z;
			// �����ٶ�
			float dvx_tan = dvx - norm_v * norm_x;
			float dvy_tan = dvy - norm_v * norm_y;
			float dvz_tan = dvz - norm_v * norm_z;
			// ������С
			float factor = -a * delta;
			force_x += factor * norm_x - c * dvx_tan;
			force_y += factor * norm_y - c * dvy_tan;
			force_z += factor * norm_z - c * dvz_tan;
		}
	}

	// �����ٶ�
	nvx[index] = vx[index] + force_x / mass;
	nvy[index] = vy[index] + force_y / mass + g * interval;
	nvz[index] = vz[index] + force_z / mass;

	// ��������
	nx[index] += nvx[index] * interval;
	ny[index] += nvy[index] * interval;
	nz[index] += nvz[index] * interval;

	// ���紦��
	if (nx[index] < -x_maxm + r[index]) {
		nx[index] = -x_maxm + r[index];
		nvx[index] *= decay;
	}
	if (nx[index] > x_maxm - r[index]) {
		nx[index] = x_maxm - r[index];
		nvx[index] *= decay;
	}
	if (ny[index] < -y_maxm + r[index]) {
		ny[index] = -y_maxm + r[index];
		nvy[index] *= decay;
	}
	if (ny[index] > y_maxm - r[index]) {
		ny[index] = y_maxm - r[index];
		nvy[index] *= decay;
	}
	if (nz[index] < -z_maxm + r[index]) {
		nz[index] = -z_maxm + r[index];
		nvz[index] *= decay;
	}
	if (nz[index] > z_maxm - r[index]) {
		nz[index] = z_maxm - r[index];
		nvz[index] *= decay;
	}

	// д��λ�ú��ٶ�
	barrier(CLK_LOCAL_MEM_FENCE);
	x[index] = nx[index];
	y[index] = ny[index];
	z[index] = nz[index];
	vx[index] = nvx[index];
	vy[index] = nvy[index];
	vz[index] = nvz[index];
}