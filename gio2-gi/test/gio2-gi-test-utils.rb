# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "test-unit"
require "test/unit/notify"

module GioGITestUtils
  def omit_not_implemented
    omit("NotImplemented: free GIArgument")
  end
end
