<!-- 
Echo Open licensed under GPLv2 (http://www.gnu.org/licenses/gpl-2.0.html)
Modified Date: June 30, 2011.
-->

<p><?php print $intro_text; ?></p>

<div class="my-form-wrapper">
  <?php print $rendered; ?>
</div>
<div class="form-item">
<label for="edit-name">Usernameasd: <span class="form-required" title="This field is required.">*</span></label>
<input type="text" maxlength="60" name="name" id="edit-name"  size="30" value="" tabindex="1" class="form-text required" />
<div class="description">enter your username</div>
</div>
<div class="form-item">
<label for="edit-pass">Password: <span class="form-required" title="This field is required.">*</span></label>
<input type="password" name="pass" id="edit-pass"  size="40"  tabindex="2" class="form-text required" />
<div class="description">enter your password</div>
</div>
<input type="hidden" name="form_id" id="edit-user-login" value="user_login"  />
<input type="submit" name="op" id="edit-submit" value="Log in"  tabindex="3" class="form-submit" />
<p><a class="textlink" href="?q=user/password">Forgotten your Password?</a></p>